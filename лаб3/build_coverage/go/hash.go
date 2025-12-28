package main

import (
	"encoding/binary"
	"encoding/json"
	"fmt"
	"strings"
)

type ChainNode struct {
	Key  string      `json:"key"`
	Next *ChainNode  `json:"next"`
}

type ChainHashTable struct {
	Name         string       `json:"name"`
	Table        []*ChainNode `json:"table"`
	TableSize    int          `json:"table_size"`
	ElementCount int          `json:"element_count"`
}

func NewChainHashTable(size int) *ChainHashTable {
	if size <= 0 {
		size = 10
	}
	
	table := make([]*ChainNode, size)
	return &ChainHashTable{
		Table:     table,
		TableSize: size,
	}
}

func (ht *ChainHashTable) hash(key string) int {
	hash := 5381
	for _, c := range key {
		hash = ((hash << 5) + hash) + int(c)
	}
	if ht.TableSize == 0 {
		return 0
	}
	result := hash % ht.TableSize
	if result < 0 {
		result = -result
	}
	return result
}

func (ht *ChainHashTable) Insert(key string) {
	index := ht.hash(key)
	current := ht.Table[index]
	
	for current != nil {
		if current.Key == key {
			return
		}
		current = current.Next
	}
	
	newNode := &ChainNode{Key: key, Next: ht.Table[index]}
	ht.Table[index] = newNode
	ht.ElementCount++
}

func (ht *ChainHashTable) Contains(key string) bool {
	index := ht.hash(key)
	if index < 0 || index >= ht.TableSize {
		return false
	}
	
	current := ht.Table[index]
	
	for current != nil {
		if current.Key == key {
			return true
		}
		current = current.Next
	}
	return false
}

func (ht *ChainHashTable) Remove(key string) bool {
	index := ht.hash(key)
	if index < 0 || index >= ht.TableSize {
		return false
	}
	
	current := ht.Table[index]
	var prev *ChainNode
	
	for current != nil {
		if current.Key == key {
			if prev == nil {
				ht.Table[index] = current.Next
			} else {
				prev.Next = current.Next
			}
			ht.ElementCount--
			return true
		}
		prev = current
		current = current.Next
	}
	return false
}

func (ht *ChainHashTable) Print() {
	fmt.Printf("Хэш-таблица (метод цепочек) '%s':\n", ht.Name)
	for i := 0; i < ht.TableSize; i++ {
		if ht.Table[i] != nil {
			fmt.Printf("  Ячейка %d: ", i)
			var keys []string
			current := ht.Table[i]
			for current != nil {
				keys = append(keys, current.Key)
				current = current.Next
			}
			fmt.Println(strings.Join(keys, " -> "))
		}
	}
}

func (ht *ChainHashTable) GetAllElements() []string {
	var elements []string
	for i := 0; i < ht.TableSize; i++ {
		current := ht.Table[i]
		for current != nil {
			elements = append(elements, current.Key)
			current = current.Next
		}
	}
	return elements
}

func (ht *ChainHashTable) MarshalJSON() ([]byte, error) {
	type Alias ChainHashTable
	return json.Marshal(&struct {
		*Alias
		Elements []string `json:"elements"`
	}{
		Alias:    (*Alias)(ht),
		Elements: ht.GetAllElements(),
	})
}

func (ht *ChainHashTable) UnmarshalJSON(data []byte) error {
	type Alias ChainHashTable
	aux := &struct {
		Elements []string `json:"elements"`
		*Alias
	}{
		Alias: (*Alias)(ht),
	}
	
	if err := json.Unmarshal(data, aux); err != nil {
		return err
	}
	
	ht.Table = make([]*ChainNode, ht.TableSize)
	ht.ElementCount = 0
	
	if aux.Elements != nil {
		for _, elem := range aux.Elements {
			ht.Insert(elem)
		}
	}
	
	return nil
}

func (ht *ChainHashTable) SerializeBinary() []byte {
	elements := ht.GetAllElements()
	buf := make([]byte, 0)
	
	nameBytes := []byte(ht.Name)
	nameLen := len(nameBytes)
	buf = binary.BigEndian.AppendUint32(buf, uint32(nameLen))
	buf = append(buf, nameBytes...)
	
	buf = binary.BigEndian.AppendUint32(buf, uint32(ht.TableSize))
	
	buf = binary.BigEndian.AppendUint32(buf, uint32(len(elements)))
	
	for _, elem := range elements {
		elemBytes := []byte(elem)
		elemLen := len(elemBytes)
		buf = binary.BigEndian.AppendUint32(buf, uint32(elemLen))
		buf = append(buf, elemBytes...)
	}
	
	return buf
}

func (ht *ChainHashTable) DeserializeBinary(data []byte) error {
	offset := 0
	
	nameLen := binary.BigEndian.Uint32(data[offset:])
	offset += 4
	ht.Name = string(data[offset : offset+int(nameLen)])
	offset += int(nameLen)
	
	ht.TableSize = int(binary.BigEndian.Uint32(data[offset:]))
	offset += 4
	
	ht.Table = make([]*ChainNode, ht.TableSize)
	ht.ElementCount = 0
	
	elemCount := int(binary.BigEndian.Uint32(data[offset:]))
	offset += 4
	
	for i := 0; i < elemCount; i++ {
		elemLen := int(binary.BigEndian.Uint32(data[offset:]))
		offset += 4
		elem := string(data[offset : offset+elemLen])
		offset += elemLen
		ht.Insert(elem)
	}
	
	return nil
}

type OpenHashTable struct {
	Name         string   `json:"name"`
	Table        []string `json:"table"`
	TableSize    int      `json:"table_size"`
	ElementCount int      `json:"element_count"`
}

func NewOpenHashTable(size int) *OpenHashTable {
	if size <= 0 {
		size = 10
	}
	
	table := make([]string, size)
	for i := range table {
		table[i] = ""
	}
	
	return &OpenHashTable{
		Table:     table,
		TableSize: size,
	}
}

func (ht *OpenHashTable) hash(key string) int {
	hash := 0
	for _, c := range key {
		hash = (hash * 31) + int(c)
	}
	if ht.TableSize == 0 {
		return 0
	}
	result := hash % ht.TableSize
	if result < 0 {
		result = -result
	}
	return result
}

func (ht *OpenHashTable) Insert(key string) {
	if ht.ElementCount >= ht.TableSize {
		fmt.Println("Таблица переполнена!")
		return
	}
	
	for attempt := 0; attempt < ht.TableSize; attempt++ {
		index := (ht.hash(key) + attempt) % ht.TableSize
		if ht.Table[index] == "" {
			ht.Table[index] = key
			ht.ElementCount++
			return
		} else if ht.Table[index] == key {
			return
		}
	}
}

func (ht *OpenHashTable) Contains(key string) bool {
	for attempt := 0; attempt < ht.TableSize; attempt++ {
		index := (ht.hash(key) + attempt) % ht.TableSize
		if index < 0 || index >= ht.TableSize {
			return false
		}
		if ht.Table[index] == "" {
			return false
		} else if ht.Table[index] == key {
			return true
		}
	}
	return false
}

func (ht *OpenHashTable) Remove(key string) bool {
	for attempt := 0; attempt < ht.TableSize; attempt++ {
		index := (ht.hash(key) + attempt) % ht.TableSize
		if index < 0 || index >= ht.TableSize {
			return false
		}
		if ht.Table[index] != "" && ht.Table[index] == key {
			ht.Table[index] = ""
			ht.ElementCount--
			return true
		}
	}
	return false
}

func (ht *OpenHashTable) Print() {
	fmt.Printf("Хэш-таблица (открытая адресация) '%s':\n", ht.Name)
	for i := 0; i < ht.TableSize; i++ {
		if ht.Table[i] != "" {
			fmt.Printf("  Ячейка %d: %s\n", i, ht.Table[i])
		}
	}
}

func (ht *OpenHashTable) GetAllElements() []string {
	var elements []string
	for i := 0; i < ht.TableSize; i++ {
		if ht.Table[i] != "" {
			elements = append(elements, ht.Table[i])
		}
	}
	return elements
}

func (ht *OpenHashTable) MarshalJSON() ([]byte, error) {
	type Alias OpenHashTable
	return json.Marshal(&struct {
		*Alias
		Elements []string `json:"elements"`
	}{
		Alias:    (*Alias)(ht),
		Elements: ht.GetAllElements(),
	})
}

func (ht *OpenHashTable) UnmarshalJSON(data []byte) error {
	type Alias OpenHashTable
	aux := &struct {
		Elements []string `json:"elements"`
		*Alias
	}{
		Alias: (*Alias)(ht),
	}
	
	if err := json.Unmarshal(data, aux); err != nil {
		return err
	}
	
	ht.Table = make([]string, ht.TableSize)
	ht.ElementCount = 0
	
	if aux.Elements != nil {
		for _, elem := range aux.Elements {
			ht.Insert(elem)
		}
	}
	
	return nil
}

func (ht *OpenHashTable) SerializeBinary() []byte {
	elements := ht.GetAllElements()
	buf := make([]byte, 0)
	
	nameBytes := []byte(ht.Name)
	nameLen := len(nameBytes)
	buf = binary.BigEndian.AppendUint32(buf, uint32(nameLen))
	buf = append(buf, nameBytes...)
	
	buf = binary.BigEndian.AppendUint32(buf, uint32(ht.TableSize))
	
	buf = binary.BigEndian.AppendUint32(buf, uint32(len(elements)))
	
	for _, elem := range elements {
		elemBytes := []byte(elem)
		elemLen := len(elemBytes)
		buf = binary.BigEndian.AppendUint32(buf, uint32(elemLen))
		buf = append(buf, elemBytes...)
	}
	
	return buf
}

func (ht *OpenHashTable) DeserializeBinary(data []byte) error {
	offset := 0
	
	nameLen := binary.BigEndian.Uint32(data[offset:])
	offset += 4
	ht.Name = string(data[offset : offset+int(nameLen)])
	offset += int(nameLen)
	
	ht.TableSize = int(binary.BigEndian.Uint32(data[offset:]))
	offset += 4
	
	ht.Table = make([]string, ht.TableSize)
	ht.ElementCount = 0
	
	elemCount := int(binary.BigEndian.Uint32(data[offset:]))
	offset += 4
	
	for i := 0; i < elemCount; i++ {
		elemLen := int(binary.BigEndian.Uint32(data[offset:]))
		offset += 4
		elem := string(data[offset : offset+elemLen])
		offset += elemLen
		ht.Insert(elem)
	}
	
	return nil
}
