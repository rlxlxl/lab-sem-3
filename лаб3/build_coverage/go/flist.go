package main

import (
	"encoding/binary"
	"encoding/json"
	"fmt"
	"strings"
)

type Node struct {
	Key  string `json:"key"`
	Next *Node  `json:"next"`
}

type ForwardList struct {
	Name string `json:"name"`
	Head *Node  `json:"head"`
}

func NewForwardList() *ForwardList {
	return &ForwardList{}
}

func (fl *ForwardList) AddToEnd(key string) {
	newNode := &Node{Key: key}
	if fl.Head == nil {
		fl.Head = newNode
		return
	}
	
	current := fl.Head
	for current.Next != nil {
		current = current.Next
	}
	current.Next = newNode
}

func (fl *ForwardList) AddToBeginning(key string) {
	newNode := &Node{Key: key, Next: fl.Head}
	fl.Head = newNode
}

func (fl *ForwardList) AddAfterNode(targetNode *Node, key string) {
	if targetNode == nil {
		return
	}
	
	newNode := &Node{Key: key}
	newNode.Next = targetNode.Next
	targetNode.Next = newNode
}

func (fl *ForwardList) AddBeforeNode(targetNode *Node, key string) {
	if targetNode == nil {
		return
	}
	
	if fl.Head == targetNode {
		fl.AddToBeginning(key)
		return
	}
	
	current := fl.Head
	for current != nil && current.Next != targetNode {
		current = current.Next
	}
	
	if current != nil {
		newNode := &Node{Key: key}
		newNode.Next = targetNode
		current.Next = newNode
	}
}

func (fl *ForwardList) GetFirst() (string, error) {
	if fl.Head == nil {
		return "", fmt.Errorf("список пуст")
	}
	return fl.Head.Key, nil
}

func (fl *ForwardList) DeleteByValue(value string) bool {
	if fl.Head == nil {
		return false
	}
	
	if fl.Head.Key == value {
		fl.Head = fl.Head.Next
		return true
	}
	
	current := fl.Head
	for current.Next != nil && current.Next.Key != value {
		current = current.Next
	}
	
	if current.Next != nil && current.Next.Key == value {
		current.Next = current.Next.Next
		return true
	}
	
	return false
}

func (fl *ForwardList) FindNodeByValue(value string) *Node {
	current := fl.Head
	for current != nil {
		if current.Key == value {
			return current
		}
		current = current.Next
	}
	return nil
}

func (fl *ForwardList) Print() {
	if fl.Head == nil {
		fmt.Printf("Односвязный список %s пуст\n", fl.Name)
		return
	}
	
	var builder strings.Builder
	builder.WriteString(fmt.Sprintf("Односвязный список %s: ", fl.Name))
	
	current := fl.Head
	for current != nil {
		builder.WriteString(current.Key)
		if current.Next != nil {
			builder.WriteString(" ")
		}
		current = current.Next
	}
	
	fmt.Println(builder.String())
}

func (fl *ForwardList) GetAllElements() []string {
	var elements []string
	current := fl.Head
	for current != nil {
		elements = append(elements, current.Key)
		current = current.Next
	}
	return elements
}

func (fl *ForwardList) SetAllElements(elements []string) {
	fl.Head = nil
	for _, elem := range elements {
		fl.AddToEnd(elem)
	}
}

// сериализация
func (fl *ForwardList) MarshalJSON() ([]byte, error) {
	type Alias ForwardList
	return json.Marshal(&struct {
		*Alias
		Elements []string `json:"elements"`
	}{
		Alias:    (*Alias)(fl),
		Elements: fl.GetAllElements(),
	})
}

// десериализация
func (fl *ForwardList) UnmarshalJSON(data []byte) error {
	type Alias ForwardList
	aux := &struct {
		Elements []string `json:"elements"`
		*Alias
	}{
		Alias: (*Alias)(fl),
	}
	
	if err := json.Unmarshal(data, aux); err != nil {
		return err
	}
	
	if aux.Elements != nil {
		fl.SetAllElements(aux.Elements)
	}
	
	return nil
}

// бинарная сериализация
func (fl *ForwardList) SerializeBinary() []byte {
	elements := fl.GetAllElements()
	buf := make([]byte, 0)
	
	// Имя
	nameBytes := []byte(fl.Name)
	nameLen := len(nameBytes)
	buf = binary.BigEndian.AppendUint32(buf, uint32(nameLen))
	buf = append(buf, nameBytes...)
	
	// Количество элементов
	buf = binary.BigEndian.AppendUint32(buf, uint32(len(elements)))
	
	// Элементы
	for _, elem := range elements {
		elemBytes := []byte(elem)
		elemLen := len(elemBytes)
		buf = binary.BigEndian.AppendUint32(buf, uint32(elemLen))
		buf = append(buf, elemBytes...)
	}
	
	return buf
}

// бинарная десериализация
func (fl *ForwardList) DeserializeBinary(data []byte) error {
	offset := 0
	
	// Имя
	nameLen := binary.BigEndian.Uint32(data[offset:])
	offset += 4
	fl.Name = string(data[offset : offset+int(nameLen)])
	offset += int(nameLen)
	
	// Количество элементов
	elemCount := int(binary.BigEndian.Uint32(data[offset:]))
	offset += 4
	
	// Элементы
	var elements []string
	for i := 0; i < elemCount; i++ {
		elemLen := int(binary.BigEndian.Uint32(data[offset:]))
		offset += 4
		elem := string(data[offset : offset+elemLen])
		offset += elemLen
		elements = append(elements, elem)
	}
	
	fl.SetAllElements(elements)
	return nil
}
