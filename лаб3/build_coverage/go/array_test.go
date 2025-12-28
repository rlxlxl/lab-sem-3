package main

import (
	"encoding/json"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestDynamicArray_New(t *testing.T) {
	tests := []struct {
		name     string
		capacity int
		expected int
	}{
		{"positive capacity", 10, 10},
		{"zero capacity", 0, 1},
		{"negative capacity", -5, 1},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			arr := NewDynamicArray(tt.capacity)
			assert.Equal(t, tt.expected, arr.Capacity)
			assert.Equal(t, 0, arr.Size)
			assert.NotNil(t, arr.Data)
		})
	}
}

func TestDynamicArray_AddToEnd(t *testing.T) {
	arr := NewDynamicArray(2)
	arr.Name = "test-array"

	arr.AddToEnd("first")
	assert.Equal(t, 1, arr.Size)
	assert.Equal(t, "first", arr.Data[0])

	arr.AddToEnd("second")
	assert.Equal(t, 2, arr.Size)
	assert.Equal(t, "second", arr.Data[1])

	arr.AddToEnd("third")
	assert.Equal(t, 3, arr.Size)
	assert.Equal(t, 4, arr.Capacity)
	assert.Equal(t, "third", arr.Data[2])
}

func TestDynamicArray_AddAtIndex(t *testing.T) {
	arr := NewDynamicArray(5)
	arr.Name = "test-array"

	arr.AddToEnd("A")
	arr.AddToEnd("B")
	arr.AddToEnd("D")

	err := arr.AddAtIndex(2, "C")
	require.NoError(t, err)
	assert.Equal(t, 4, arr.Size)
	assert.Equal(t, "C", arr.Data[2])

	err = arr.AddAtIndex(0, "start")
	require.NoError(t, err)
	assert.Equal(t, 5, arr.Size)
	assert.Equal(t, "start", arr.Data[0])

	err = arr.AddAtIndex(5, "end")
	require.NoError(t, err)
	assert.Equal(t, 6, arr.Size)
	assert.Equal(t, "end", arr.Data[5])

	err = arr.AddAtIndex(-1, "invalid")
	assert.Error(t, err)
	assert.Equal(t, "индекс вне границ массива", err.Error())

	err = arr.AddAtIndex(10, "invalid")
	assert.Error(t, err)
	assert.Equal(t, "индекс вне границ массива", err.Error())
}

func TestDynamicArray_RemoveAtIndex(t *testing.T) {
	arr := NewDynamicArray(5)
	arr.Name = "test-array"

	arr.AddToEnd("A")
	arr.AddToEnd("B")
	arr.AddToEnd("C")
	arr.AddToEnd("D")

	err := arr.RemoveAtIndex(1)
	require.NoError(t, err)
	assert.Equal(t, 3, arr.Size)
	assert.Equal(t, "C", arr.Data[1])

	err = arr.RemoveAtIndex(0)
	require.NoError(t, err)
	assert.Equal(t, 2, arr.Size)
	assert.Equal(t, "C", arr.Data[0])

	err = arr.RemoveAtIndex(1)
	require.NoError(t, err)
	assert.Equal(t, 1, arr.Size)

	err = arr.RemoveAtIndex(-1)
	assert.Error(t, err)
	assert.Equal(t, "индекс вне границ массива", err.Error())

	err = arr.RemoveAtIndex(5)
	assert.Error(t, err)
	assert.Equal(t, "индекс вне границ массива", err.Error())
}

func TestDynamicArray_GetElement(t *testing.T) {
	arr := NewDynamicArray(5)
	arr.Name = "test-array"

	arr.AddToEnd("first")
	arr.AddToEnd("second")
	arr.AddToEnd("third")

	val, err := arr.GetElement(0)
	require.NoError(t, err)
	assert.Equal(t, "first", val)

	val, err = arr.GetElement(2)
	require.NoError(t, err)
	assert.Equal(t, "third", val)

	_, err = arr.GetElement(-1)
	assert.Error(t, err)
	assert.Equal(t, "индекс вне границ массива", err.Error())

	_, err = arr.GetElement(5)
	assert.Error(t, err)
	assert.Equal(t, "индекс вне границ массива", err.Error())
}

func TestDynamicArray_SetElement(t *testing.T) {
	arr := NewDynamicArray(5)
	arr.Name = "test-array"

	arr.AddToEnd("old1")
	arr.AddToEnd("old2")

	err := arr.SetElement(1, "new2")
	require.NoError(t, err)
	assert.Equal(t, "new2", arr.Data[1])

	err = arr.SetElement(-1, "invalid")
	assert.Error(t, err)
	assert.Equal(t, "индекс вне границ массива", err.Error())

	err = arr.SetElement(5, "invalid")
	assert.Error(t, err)
	assert.Equal(t, "индекс вне границ массива", err.Error())
}

func TestDynamicArray_GetLength(t *testing.T) {
	arr := NewDynamicArray(5)
	arr.Name = "test-array"

	assert.Equal(t, 0, arr.GetLength())

	arr.AddToEnd("item1")
	assert.Equal(t, 1, arr.GetLength())

	arr.AddToEnd("item2")
	arr.AddToEnd("item3")
	assert.Equal(t, 3, arr.GetLength())

	arr.RemoveAtIndex(1)
	assert.Equal(t, 2, arr.GetLength())
}

func TestDynamicArray_JSONSerialization(t *testing.T) {
	arr := NewDynamicArray(5)
	arr.Name = "test-array"
	arr.AddToEnd("first")
	arr.AddToEnd("second")
	arr.AddToEnd("third")

	data, err := json.Marshal(arr)
	require.NoError(t, err)
	assert.NotEmpty(t, data)

	var newArr DynamicArray
	err = json.Unmarshal(data, &newArr)
	require.NoError(t, err)

	assert.Equal(t, arr.Name, newArr.Name)
	assert.Equal(t, arr.Size, newArr.Size)
	assert.Equal(t, arr.Capacity, newArr.Capacity)
	
	for i := 0; i < arr.Size; i++ {
		assert.Equal(t, arr.Data[i], newArr.Data[i])
	}
}

func TestDynamicArray_BinarySerialization(t *testing.T) {
	arr := NewDynamicArray(5)
	arr.Name = "test-array"
	arr.AddToEnd("first")
	arr.AddToEnd("second")
	arr.AddToEnd("third")

	binaryData := arr.SerializeBinary()
	assert.NotEmpty(t, binaryData)

	newArr := NewDynamicArray(1)
	err := newArr.DeserializeBinary(binaryData)
	require.NoError(t, err)

	assert.Equal(t, arr.Name, newArr.Name)
	assert.Equal(t, arr.Size, newArr.Size)
	
	for i := 0; i < arr.Size; i++ {
		assert.Equal(t, arr.Data[i], newArr.Data[i])
	}
}

func TestDynamicArray_Print(t *testing.T) {
	arr := NewDynamicArray(3)
	arr.Name = "test-array"
	
	arr.Print()

	arr.AddToEnd("A")
	arr.AddToEnd("B")
	arr.AddToEnd("C")
	arr.Print()
}
