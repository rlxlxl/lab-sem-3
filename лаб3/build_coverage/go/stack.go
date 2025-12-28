package main

import (
	"encoding/binary"
	"encoding/json"
	"fmt"
	"strings"
)

type Stack struct {
	Name string `json:"name"`
	Top  *Node  `json:"top"`
}

func NewStack() *Stack {
	return &Stack{}
}

func (s *Stack) Push(data string) {
	newNode := &Node{Key: data, Next: s.Top}
	s.Top = newNode
}

func (s *Stack) Pop() (string, error) {
	if s.Top == nil {
		return "", fmt.Errorf("извлечение из пустого стека")
	}
	
	data := s.Top.Key
	s.Top = s.Top.Next
	return data, nil
}

func (s *Stack) Peek() (string, error) {
	if s.Top == nil {
		return "", fmt.Errorf("стек пустой")
	}
	return s.Top.Key, nil
}

func (s *Stack) Print() {
	if s.Top == nil {
		fmt.Printf("Стек %s пустой\n", s.Name)
		return
	}
	
	var builder strings.Builder
	builder.WriteString(fmt.Sprintf("Стек %s: ", s.Name))
	
	current := s.Top
	for current != nil {
		builder.WriteString(current.Key)
		if current.Next != nil {
			builder.WriteString(" ")
		}
		current = current.Next
	}
	
	fmt.Println(builder.String())
}

func (s *Stack) GetAllElements() []string {
	var elements []string
	current := s.Top
	for current != nil {
		elements = append(elements, current.Key)
		current = current.Next
	}
	return elements
}

func (s *Stack) SetAllElements(elements []string) {
	s.Top = nil
	for i := len(elements) - 1; i >= 0; i-- {
		s.Push(elements[i])
	}
}

// сериализация
func (s *Stack) MarshalJSON() ([]byte, error) {
	type Alias Stack
	return json.Marshal(&struct {
		*Alias
		Elements []string `json:"elements"`
	}{
		Alias:    (*Alias)(s),
		Elements: s.GetAllElements(),
	})
}

// десериализация
func (s *Stack) UnmarshalJSON(data []byte) error {
	type Alias Stack
	aux := &struct {
		Elements []string `json:"elements"`
		*Alias
	}{
		Alias: (*Alias)(s),
	}
	
	if err := json.Unmarshal(data, aux); err != nil {
		return err
	}
	
	if aux.Elements != nil {
		s.SetAllElements(aux.Elements)
	}
	
	return nil
}

// бинарная сериализация
func (s *Stack) SerializeBinary() []byte {
	elements := s.GetAllElements()
	buf := make([]byte, 0)
	
	nameBytes := []byte(s.Name)
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

// бинарная десериализация
func (s *Stack) DeserializeBinary(data []byte) error {
	offset := 0
	
	nameLen := binary.BigEndian.Uint32(data[offset:])
	offset += 4
	s.Name = string(data[offset : offset+int(nameLen)])
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
	
	s.SetAllElements(elements)
	return nil
}
