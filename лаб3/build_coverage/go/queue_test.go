package main

import (
	"encoding/json"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestQueue_New(t *testing.T) {
	queue := NewQueue()
	assert.Nil(t, queue.Front)
	assert.Nil(t, queue.Rear)
	assert.Empty(t, queue.Name)
}

func TestQueue_Enqueue(t *testing.T) {
	queue := NewQueue()
	queue.Name = "test-queue"

	queue.Enqueue("first")
	assert.NotNil(t, queue.Front)
	assert.NotNil(t, queue.Rear)
	assert.Equal(t, queue.Front, queue.Rear)
	assert.Equal(t, "first", queue.Front.Key)

	queue.Enqueue("second")
	assert.Equal(t, "first", queue.Front.Key)
	assert.Equal(t, "second", queue.Rear.Key)
	assert.Equal(t, queue.Rear, queue.Front.Next)

	queue.Enqueue("third")
	assert.Equal(t, "first", queue.Front.Key)
	assert.Equal(t, "third", queue.Rear.Key)
	assert.Equal(t, "second", queue.Front.Next.Key)
}

func TestQueue_Dequeue(t *testing.T) {
	queue := NewQueue()
	queue.Name = "test-queue"

	val, err := queue.Dequeue()
	assert.Error(t, err)
	assert.Equal(t, "извлечение из пустой очереди", err.Error())
	assert.Empty(t, val)

	queue.Enqueue("A")
	queue.Enqueue("B")
	queue.Enqueue("C")

	val, err = queue.Dequeue()
	require.NoError(t, err)
	assert.Equal(t, "A", val)
	assert.Equal(t, "B", queue.Front.Key)

	val, err = queue.Dequeue()
	require.NoError(t, err)
	assert.Equal(t, "B", val)
	assert.Equal(t, "C", queue.Front.Key)
	assert.Equal(t, queue.Front, queue.Rear)

	val, err = queue.Dequeue()
	require.NoError(t, err)
	assert.Equal(t, "C", val)
	assert.Nil(t, queue.Front)
	assert.Nil(t, queue.Rear)

	_, err = queue.Dequeue()
	assert.Error(t, err)
}

func TestQueue_Peek(t *testing.T) {
	queue := NewQueue()
	queue.Name = "test-queue"

	val, err := queue.Peek()
	assert.Error(t, err)
	assert.Equal(t, "чтение из пустой очереди", err.Error())
	assert.Empty(t, val)

	queue.Enqueue("A")
	val, err = queue.Peek()
	require.NoError(t, err)
	assert.Equal(t, "A", val)
	assert.Equal(t, "A", queue.Front.Key)

	queue.Enqueue("B")
	val, err = queue.Peek()
	require.NoError(t, err)
	assert.Equal(t, "A", val)

	queue.Dequeue()
	val, err = queue.Peek()
	require.NoError(t, err)
	assert.Equal(t, "B", val)
}

func TestQueue_GetAllElements(t *testing.T) {
	queue := NewQueue()
	queue.Name = "test-queue"

	elements := queue.GetAllElements()
	assert.Empty(t, elements)

	queue.Enqueue("A")
	queue.Enqueue("B")
	queue.Enqueue("C")

	elements = queue.GetAllElements()
	assert.Equal(t, []string{"A", "B", "C"}, elements)

	queue.Dequeue()
	elements = queue.GetAllElements()
	assert.Equal(t, []string{"B", "C"}, elements)
}

func TestQueue_SetAllElements(t *testing.T) {
	queue := NewQueue()
	queue.Name = "test-queue"

	elements := []string{"X", "Y", "Z"}
	queue.SetAllElements(elements)

	assert.Equal(t, "X", queue.Front.Key)
	assert.Equal(t, "Y", queue.Front.Next.Key)
	assert.Equal(t, "Z", queue.Rear.Key)

	queue.SetAllElements([]string{})
	assert.Nil(t, queue.Front)
	assert.Nil(t, queue.Rear)
}

func TestQueue_JSONSerialization(t *testing.T) {
	queue := NewQueue()
	queue.Name = "test-queue"
	queue.Enqueue("first")
	queue.Enqueue("second")
	queue.Enqueue("third")

	data, err := json.Marshal(queue)
	require.NoError(t, err)
	assert.NotEmpty(t, data)

	var newQueue Queue
	err = json.Unmarshal(data, &newQueue)
	require.NoError(t, err)

	assert.Equal(t, queue.Name, newQueue.Name)
	assert.Equal(t, queue.GetAllElements(), newQueue.GetAllElements())
}

func TestQueue_BinarySerialization(t *testing.T) {
	queue := NewQueue()
	queue.Name = "test-queue"
	queue.Enqueue("first")
	queue.Enqueue("second")
	queue.Enqueue("third")

	binaryData := queue.SerializeBinary()
	assert.NotEmpty(t, binaryData)

	newQueue := NewQueue()
	err := newQueue.DeserializeBinary(binaryData)
	require.NoError(t, err)

	assert.Equal(t, queue.Name, newQueue.Name)
	assert.Equal(t, queue.GetAllElements(), newQueue.GetAllElements())
}

func TestQueue_Print(t *testing.T) {
	queue := NewQueue()
	queue.Name = "test-queue"
	
	queue.Print()

	queue.Enqueue("A")
	queue.Enqueue("B")
	queue.Enqueue("C")
	queue.Print()
}
