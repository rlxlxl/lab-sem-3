package main

import (
	"encoding/binary"
	"encoding/json"
	"fmt"
	"strings"
)

type DNode struct {
	Key  string `json:"key"`
	Next *DNode `json:"-"`
	Prev *DNode `json:"-"`
}

type DoubleList struct {
	Name string  `json:"name"`
	Head *DNode  `json:"-"`
	Tail *DNode  `json:"-"`
}

func NewDoubleList() *DoubleList {
	return &DoubleList{}
}

func (dl *DoubleList) AddToEnd(key string) {
	newNode := &DNode{Key: key}
	
	if dl.Head == nil {
		dl.Head = newNode
		dl.Tail = newNode
		return
	}
	
	newNode.Prev = dl.Tail
	dl.Tail.Next = newNode
	dl.Tail = newNode
}

func (dl *DoubleList) AddToBeginning(key string) {
	newNode := &DNode{Key: key}
	
	if dl.Head == nil {
		dl.Head = newNode
		dl.Tail = newNode
		return
	}
	
	newNode.Next = dl.Head
	dl.Head.Prev = newNode
	dl.Head = newNode
}

func (dl *DoubleList) FindNodeByValue(value string) *DNode {
	current := dl.Head
	for current != nil {
		if current.Key == value {
			return current
		}
		current = current.Next
	}
	return nil
}

func (dl *DoubleList) GetFirst() (string, error) {
	if dl.Head == nil {
		return "", fmt.Errorf("список пуст")
	}
	return dl.Head.Key, nil
}

func (dl *DoubleList) DeleteByValue(value string) bool {
	if dl.Head == nil {
		return false
	}
	
	if dl.Head.Key == value {
		dl.Head = dl.Head.Next
		if dl.Head != nil {
			dl.Head.Prev = nil
		} else {
			dl.Tail = nil
		}
		return true
	}
	
	if dl.Tail.Key == value {
		dl.Tail = dl.Tail.Prev
		dl.Tail.Next = nil
		return true
	}
	
	current := dl.Head
	for current != nil && current.Key != value {
		current = current.Next
	}
	
	if current != nil {
		current.Prev.Next = current.Next
		if current.Next != nil {
			current.Next.Prev = current.Prev
		}
		return true
	}
	
	return false
}

func (dl *DoubleList) Print() {
	if dl.Head == nil {
		fmt.Printf("Двусвязный список %s пуст\n", dl.Name)
		return
	}
	
	var builder strings.Builder
	builder.WriteString(fmt.Sprintf("Двусвязный список %s: ", dl.Name))
	
	current := dl.Head
	for current != nil {
		builder.WriteString(current.Key)
		if current.Next != nil {
			builder.WriteString(" ")
		}
		current = current.Next
	}
	
	fmt.Println(builder.String())
}

func (dl *DoubleList) AddAfterNode(targetNode *DNode, key string) {
	if targetNode == nil {
		return
	}
	
	if targetNode == dl.Tail {
		dl.AddToEnd(key)
		return
	}
	
	newNode := &DNode{Key: key}
	newNode.Prev = targetNode
	newNode.Next = targetNode.Next
	targetNode.Next.Prev = newNode
	targetNode.Next = newNode
}

func (dl *DoubleList) AddBeforeNode(targetNode *DNode, key string) {
	if targetNode == nil {
		return
	}
	
	if targetNode == dl.Head {
		dl.AddToBeginning(key)
		return
	}
	
	newNode := &DNode{Key: key}
	newNode.Next = targetNode
	newNode.Prev = targetNode.Prev
	targetNode.Prev.Next = newNode
	targetNode.Prev = newNode
}

func (dl *DoubleList) GetAllElements() []string {
	var elements []string
	current := dl.Head
	for current != nil {
		elements = append(elements, current.Key)
		current = current.Next
	}
	return elements
}

func (dl *DoubleList) SetAllElements(elements []string) {
	dl.Head = nil
	dl.Tail = nil
	for _, elem := range elements {
		dl.AddToEnd(elem)
	}
}

func (dl *DoubleList) MarshalJSON() ([]byte, error) {
	type Alias DoubleList
	return json.Marshal(&struct {
		*Alias
		Elements []string `json:"elements"`
	}{
		Alias:    (*Alias)(dl),
		Elements: dl.GetAllElements(),
	})
}

func (dl *DoubleList) UnmarshalJSON(data []byte) error {
	type Alias DoubleList
	aux := &struct {
		Elements []string `json:"elements"`
		*Alias
	}{
		Alias: (*Alias)(dl),
	}
	
	if err := json.Unmarshal(data, aux); err != nil {
		return err
	}
	
	if aux.Elements != nil {
		dl.SetAllElements(aux.Elements)
	}
	
	return nil
}

func (dl *DoubleList) SerializeBinary() []byte {
	elements := dl.GetAllElements()
	buf := make([]byte, 0)
	
	nameBytes := []byte(dl.Name)
	nameLen := len(nameBytes)
	buf = binary.BigEndian.AppendUint32(buf, uint32(nameLen))
	buf = append(buf, nameBytes...)
	
	buf = binary.BigEndian.AppendUint32(buf, uint32(len(elements)))
	
	for _, elem := range elements {
		elemBytes := []byte(elem)
		elemLen := len(elemBytes)
		buf = binary.BigEndian.AppendUint32(buf, uint32(elemLen))
		buf = append(buf, elemBytes...)
	}
	
	return buf
}

func (dl *DoubleList) DeserializeBinary(data []byte) error {
	offset := 0
	
	nameLen := binary.BigEndian.Uint32(data[offset:])
	offset += 4
	dl.Name = string(data[offset : offset+int(nameLen)])
	offset += int(nameLen)
	
	elemCount := int(binary.BigEndian.Uint32(data[offset:]))
	offset += 4
	
	var elements []string
	for i := 0; i < elemCount; i++ {
		elemLen := int(binary.BigEndian.Uint32(data[offset:]))
		offset += 4
		elem := string(data[offset : offset+elemLen])
		offset += elemLen
		elements = append(elements, elem)
	}
	
	dl.SetAllElements(elements)
	return nil
}
