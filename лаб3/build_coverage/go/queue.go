package main

import (
	"encoding/binary"
	"encoding/json"
	"fmt"
	"strings"
)

type Queue struct {
	Name  string `json:"name"`
	Front *Node  `json:"front"`
	Rear  *Node  `json:"rear"`
}

func NewQueue() *Queue {
	return &Queue{}
}

func (q *Queue) Enqueue(data string) {
	newNode := &Node{Key: data}
	if q.Rear == nil {
		q.Front = newNode
		q.Rear = newNode
	} else {
		q.Rear.Next = newNode
		q.Rear = newNode
	}
}

func (q *Queue) Dequeue() (string, error) {
	if q.Front == nil {
		return "", fmt.Errorf("извлечение из пустой очереди")
	}
	
	data := q.Front.Key
	q.Front = q.Front.Next
	if q.Front == nil {
		q.Rear = nil
	}
	
	return data, nil
}

func (q *Queue) Peek() (string, error) {
	if q.Front == nil {
		return "", fmt.Errorf("чтение из пустой очереди")
	}
	return q.Front.Key, nil
}

func (q *Queue) Print() {
	if q.Front == nil {
		fmt.Printf("Очередь %s пустая\n", q.Name)
		return
	}
	
	var builder strings.Builder
	builder.WriteString(fmt.Sprintf("Очередь %s: ", q.Name))
	
	current := q.Front
	for current != nil {
		builder.WriteString(current.Key)
		if current.Next != nil {
			builder.WriteString(" ")
		}
		current = current.Next
	}
	
	fmt.Println(builder.String())
}

func (q *Queue) GetAllElements() []string {
	var elements []string
	current := q.Front
	for current != nil {
		elements = append(elements, current.Key)
		current = current.Next
	}
	return elements
}

func (q *Queue) SetAllElements(elements []string) {
	q.Front = nil
	q.Rear = nil
	for _, elem := range elements {
		q.Enqueue(elem)
	}
}

// сериализация
func (q *Queue) MarshalJSON() ([]byte, error) {
	type Alias Queue
	return json.Marshal(&struct {
		*Alias
		Elements []string `json:"elements"`
	}{
		Alias:    (*Alias)(q),
		Elements: q.GetAllElements(),
	})
}

// десериализация
func (q *Queue) UnmarshalJSON(data []byte) error {
	type Alias Queue
	aux := &struct {
		Elements []string `json:"elements"`
		*Alias
	}{
		Alias: (*Alias)(q),
	}
	
	if err := json.Unmarshal(data, aux); err != nil {
		return err
	}
	
	if aux.Elements != nil {
		q.SetAllElements(aux.Elements)
	}
	
	return nil
}

// бинарная сериализация
func (q *Queue) SerializeBinary() []byte {
	elements := q.GetAllElements()
	buf := make([]byte, 0)
	
	nameBytes := []byte(q.Name)
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
func (q *Queue) DeserializeBinary(data []byte) error {
	offset := 0
	
	nameLen := binary.BigEndian.Uint32(data[offset:])
	offset += 4
	q.Name = string(data[offset : offset+int(nameLen)])
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
	
	q.SetAllElements(elements)
	return nil
}
