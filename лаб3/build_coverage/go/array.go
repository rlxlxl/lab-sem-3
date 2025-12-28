package main

import (
	"encoding/binary"
	"encoding/json"
	"fmt"
	"strings"
)

type DynamicArray struct {
	Name     string   `json:"name"`
	Data     []string `json:"data"`
	Capacity int      `json:"capacity"`
	Size     int      `json:"size"`
}

func NewDynamicArray(initialCapacity int) *DynamicArray {
	if initialCapacity <= 0 {
		initialCapacity = 1
	}
	
	return &DynamicArray{
		Data:     make([]string, initialCapacity),
		Capacity: initialCapacity,
		Size:     0,
	}
}

// сериализация
func (da *DynamicArray) MarshalJSON() ([]byte, error) {
	type Alias DynamicArray
	return json.Marshal(&struct {
		*Alias
		ActualData []string `json:"actual_data"`
	}{
		Alias:      (*Alias)(da),
		ActualData: da.Data[:da.Size],
	})
}

// десериализация
func (da *DynamicArray) UnmarshalJSON(data []byte) error {
	type Alias DynamicArray
	aux := &struct {
		ActualData []string `json:"actual_data"`
		*Alias
	}{
		Alias: (*Alias)(da),
	}
	
	if err := json.Unmarshal(data, aux); err != nil {
		return err
	}
	
	if aux.ActualData != nil {
		da.Size = len(aux.ActualData)
		if da.Capacity < da.Size {
			da.Capacity = da.Size
		}
		da.Data = make([]string, da.Capacity)
		copy(da.Data, aux.ActualData)
	}
	
	return nil
}

// бинарная сериализация
func (da *DynamicArray) SerializeBinary() []byte {
	buf := make([]byte, 0)
	
	// Имя
	nameBytes := []byte(da.Name)
	nameLen := len(nameBytes)
	buf = binary.BigEndian.AppendUint32(buf, uint32(nameLen))
	buf = append(buf, nameBytes...)
	
	// Размер
	buf = binary.BigEndian.AppendUint32(buf, uint32(da.Size))
	
	// Элементы
	for i := 0; i < da.Size; i++ {
		itemBytes := []byte(da.Data[i])
		itemLen := len(itemBytes)
		buf = binary.BigEndian.AppendUint32(buf, uint32(itemLen))
		buf = append(buf, itemBytes...)
	}
	
	return buf
}

// бинарная десериализация
func (da *DynamicArray) DeserializeBinary(data []byte) error {
	offset := 0
	
	// Имя
	nameLen := binary.BigEndian.Uint32(data[offset:])
	offset += 4
	da.Name = string(data[offset : offset+int(nameLen)])
	offset += int(nameLen)
	
	// Размер
	da.Size = int(binary.BigEndian.Uint32(data[offset:]))
	offset += 4
	
	// Элементы
	if da.Capacity < da.Size {
		da.Capacity = da.Size
	}
	da.Data = make([]string, da.Capacity)
	
	for i := 0; i < da.Size; i++ {
		itemLen := int(binary.BigEndian.Uint32(data[offset:]))
		offset += 4
		da.Data[i] = string(data[offset : offset+itemLen])
		offset += itemLen
	}
	
	return nil
}

func (da *DynamicArray) resizeIfNeeded() {
	if da.Size >= da.Capacity {
		newCapacity := da.Capacity * 2
		newData := make([]string, newCapacity)
		copy(newData, da.Data[:da.Size])
		da.Data = newData
		da.Capacity = newCapacity
	}
}

func (da *DynamicArray) AddToEnd(value string) {
	da.resizeIfNeeded()
	da.Data[da.Size] = value
	da.Size++
}

func (da *DynamicArray) AddAtIndex(index int, value string) error {
	if index < 0 || index > da.Size {
		return fmt.Errorf("индекс вне границ массива")
	}
	
	da.resizeIfNeeded()
	
	for i := da.Size; i > index; i-- {
		da.Data[i] = da.Data[i-1]
	}
	
	da.Data[index] = value
	da.Size++
	return nil
}

func (da *DynamicArray) RemoveAtIndex(index int) error {
	if index < 0 || index >= da.Size {
		return fmt.Errorf("индекс вне границ массива")
	}
	
	for i := index; i < da.Size-1; i++ {
		da.Data[i] = da.Data[i+1]
	}
	da.Size--
	return nil
}

func (da *DynamicArray) GetElement(index int) (string, error) {
	if index < 0 || index >= da.Size {
		return "", fmt.Errorf("индекс вне границ массива")
	}
	return da.Data[index], nil
}

func (da *DynamicArray) SetElement(index int, value string) error {
	if index < 0 || index >= da.Size {
		return fmt.Errorf("индекс вне границ массива")
	}
	da.Data[index] = value
	return nil
}

func (da *DynamicArray) GetLength() int {
	return da.Size
}

func (da *DynamicArray) Print() {
	if da.Size == 0 {
		fmt.Printf("Массив %s пуст\n", da.Name)
		return
	}
	
	fmt.Printf("Массив %s: %s\n", da.Name, strings.Join(da.Data[:da.Size], " "))
}
