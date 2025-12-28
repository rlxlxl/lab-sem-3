package main

import (
	"encoding/json"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestStack_New(t *testing.T) {
	stack := NewStack()
	assert.Nil(t, stack.Top)
	assert.Empty(t, stack.Name)
}

func TestStack_Push(t *testing.T) {
	stack := NewStack()
	stack.Name = "test-stack"

	stack.Push("first")
	assert.NotNil(t, stack.Top)
	assert.Equal(t, "first", stack.Top.Key)

	stack.Push("second")
	assert.Equal(t, "second", stack.Top.Key)
	assert.Equal(t, "first", stack.Top.Next.Key)

	stack.Push("third")
	assert.Equal(t, "third", stack.Top.Key)
	assert.Equal(t, "second", stack.Top.Next.Key)
}

func TestStack_Pop(t *testing.T) {
	stack := NewStack()
	stack.Name = "test-stack"

	val, err := stack.Pop()
	assert.Error(t, err)
	assert.Equal(t, "извлечение из пустого стека", err.Error())
	assert.Empty(t, val)

	stack.Push("A")
	stack.Push("B")
	stack.Push("C")

	val, err = stack.Pop()
	require.NoError(t, err)
	assert.Equal(t, "C", val)
	assert.Equal(t, "B", stack.Top.Key)

	val, err = stack.Pop()
	require.NoError(t, err)
	assert.Equal(t, "B", val)
	assert.Equal(t, "A", stack.Top.Key)

	val, err = stack.Pop()
	require.NoError(t, err)
	assert.Equal(t, "A", val)
	assert.Nil(t, stack.Top)

	_, err = stack.Pop()
	assert.Error(t, err)
}

func TestStack_Peek(t *testing.T) {
	stack := NewStack()
	stack.Name = "test-stack"

	val, err := stack.Peek()
	assert.Error(t, err)
	assert.Equal(t, "стек пустой", err.Error())
	assert.Empty(t, val)

	stack.Push("A")
	val, err = stack.Peek()
	require.NoError(t, err)
	assert.Equal(t, "A", val)
	assert.Equal(t, "A", stack.Top.Key)

	stack.Push("B")
	val, err = stack.Peek()
	require.NoError(t, err)
	assert.Equal(t, "B", val)
}

func TestStack_GetAllElements(t *testing.T) {
	stack := NewStack()
	stack.Name = "test-stack"

	elements := stack.GetAllElements()
	assert.Empty(t, elements)

	stack.Push("A")
	stack.Push("B")
	stack.Push("C")

	elements = stack.GetAllElements()
	assert.Equal(t, []string{"C", "B", "A"}, elements)

	stack.Pop()
	elements = stack.GetAllElements()
	assert.Equal(t, []string{"B", "A"}, elements)
}

func TestStack_SetAllElements(t *testing.T) {
	stack := NewStack()
	stack.Name = "test-stack"

	elements := []string{"X", "Y", "Z"}
	stack.SetAllElements(elements)

	assert.Equal(t, "Z", stack.Top.Key)
	assert.Equal(t, "Y", stack.Top.Next.Key)
	assert.Equal(t, "X", stack.Top.Next.Next.Key)

	stack.SetAllElements([]string{})
	assert.Nil(t, stack.Top)
}

func TestStack_JSONSerialization(t *testing.T) {
	stack := NewStack()
	stack.Name = "test-stack"
	stack.Push("first")
	stack.Push("second")
	stack.Push("third")

	data, err := json.Marshal(stack)
	require.NoError(t, err)
	assert.NotEmpty(t, data)

	var newStack Stack
	err = json.Unmarshal(data, &newStack)
	require.NoError(t, err)

	assert.Equal(t, stack.Name, newStack.Name)
	assert.Equal(t, stack.GetAllElements(), newStack.GetAllElements())
}

func TestStack_BinarySerialization(t *testing.T) {
	stack := NewStack()
	stack.Name = "test-stack"
	stack.Push("first")
	stack.Push("second")
	stack.Push("third")

	binaryData := stack.SerializeBinary()
	assert.NotEmpty(t, binaryData)

	newStack := NewStack()
	err := newStack.DeserializeBinary(binaryData)
	require.NoError(t, err)

	assert.Equal(t, stack.Name, newStack.Name)
	assert.Equal(t, stack.GetAllElements(), newStack.GetAllElements())
}

func TestStack_Print(t *testing.T) {
	stack := NewStack()
	stack.Name = "test-stack"
	
	stack.Print()

	stack.Push("A")
	stack.Push("B")
	stack.Push("C")
	stack.Print()
}
