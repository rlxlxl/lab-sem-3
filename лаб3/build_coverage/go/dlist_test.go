package main

import (
	"encoding/json"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestDoubleList_New(t *testing.T) {
	list := NewDoubleList()
	assert.Nil(t, list.Head)
	assert.Nil(t, list.Tail)
	assert.Empty(t, list.Name)
}

func TestDoubleList_AddToEnd(t *testing.T) {
	list := NewDoubleList()
	list.Name = "test-list"

	list.AddToEnd("first")
	assert.NotNil(t, list.Head)
	assert.NotNil(t, list.Tail)
	assert.Equal(t, list.Head, list.Tail)
	assert.Equal(t, "first", list.Head.Key)

	list.AddToEnd("second")
	assert.Equal(t, "first", list.Head.Key)
	assert.Equal(t, "second", list.Tail.Key)
	assert.Equal(t, list.Tail, list.Head.Next)
	assert.Equal(t, list.Head, list.Tail.Prev)

	list.AddToEnd("third")
	assert.Equal(t, "third", list.Tail.Key)
	assert.Equal(t, "second", list.Tail.Prev.Key)
}

func TestDoubleList_AddToBeginning(t *testing.T) {
	list := NewDoubleList()
	list.Name = "test-list"

	list.AddToBeginning("first")
	assert.Equal(t, "first", list.Head.Key)
	assert.Equal(t, list.Head, list.Tail)

	list.AddToBeginning("second")
	assert.Equal(t, "second", list.Head.Key)
	assert.Equal(t, "first", list.Tail.Key)
	assert.Equal(t, list.Head.Next, list.Tail)
	assert.Equal(t, list.Tail.Prev, list.Head)

	list.AddToBeginning("third")
	assert.Equal(t, "third", list.Head.Key)
	assert.Equal(t, "second", list.Head.Next.Key)
}

func TestDoubleList_FindNodeByValue(t *testing.T) {
	list := NewDoubleList()
	list.Name = "test-list"

	node := list.FindNodeByValue("test")
	assert.Nil(t, node)

	list.AddToEnd("A")
	list.AddToEnd("B")
	list.AddToEnd("C")

	node = list.FindNodeByValue("B")
	require.NotNil(t, node)
	assert.Equal(t, "B", node.Key)
	assert.Equal(t, "A", node.Prev.Key)
	assert.Equal(t, "C", node.Next.Key)

	node = list.FindNodeByValue("C")
	require.NotNil(t, node)
	assert.Equal(t, "C", node.Key)
	assert.Equal(t, "B", node.Prev.Key)
	assert.Nil(t, node.Next)

	node = list.FindNodeByValue("nonexistent")
	assert.Nil(t, node)
}

func TestDoubleList_GetFirst(t *testing.T) {
	list := NewDoubleList()
	list.Name = "test-list"

	val, err := list.GetFirst()
	assert.Error(t, err)
	assert.Equal(t, "список пуст", err.Error())
	assert.Empty(t, val)

	list.AddToEnd("first")
	val, err = list.GetFirst()
	require.NoError(t, err)
	assert.Equal(t, "first", val)

	list.AddToBeginning("new-first")
	val, err = list.GetFirst()
	require.NoError(t, err)
	assert.Equal(t, "new-first", val)
}

func TestDoubleList_DeleteByValue(t *testing.T) {
	list := NewDoubleList()
	list.Name = "test-list"

	deleted := list.DeleteByValue("test")
	assert.False(t, deleted)

	list.AddToEnd("only")
	deleted = list.DeleteByValue("only")
	assert.True(t, deleted)
	assert.Nil(t, list.Head)
	assert.Nil(t, list.Tail)

	list.AddToEnd("A")
	list.AddToEnd("B")
	list.AddToEnd("C")
	deleted = list.DeleteByValue("A")
	assert.True(t, deleted)
	assert.Equal(t, "B", list.Head.Key)
	assert.Nil(t, list.Head.Prev)

	deleted = list.DeleteByValue("C")
	assert.True(t, deleted)
	assert.Equal(t, "B", list.Tail.Key)
	assert.Nil(t, list.Tail.Next)

	list.AddToEnd("D")
	list.AddToEnd("E")
	deleted = list.DeleteByValue("D")
	assert.True(t, deleted)
	assert.Equal(t, "E", list.Head.Next.Key)
	assert.Equal(t, "B", list.Head.Next.Prev.Key)

	deleted = list.DeleteByValue("nonexistent")
	assert.False(t, deleted)
}

func TestDoubleList_AddAfterNode(t *testing.T) {
	list := NewDoubleList()
	list.Name = "test-list"

	list.AddToEnd("A")
	list.AddToEnd("C")

	nodeA := list.FindNodeByValue("A")
	require.NotNil(t, nodeA)

	list.AddAfterNode(nodeA, "B")
	assert.Equal(t, "B", nodeA.Next.Key)
	assert.Equal(t, "A", nodeA.Next.Prev.Key)
	assert.Equal(t, "C", nodeA.Next.Next.Key)

	nodeC := list.FindNodeByValue("C")
	list.AddAfterNode(nodeC, "D")
	assert.Equal(t, "D", list.Tail.Key)
	assert.Equal(t, "C", list.Tail.Prev.Key)

	list.AddAfterNode(nil, "test")
}

func TestDoubleList_AddBeforeNode(t *testing.T) {
	list := NewDoubleList()
	list.Name = "test-list"

	list.AddToEnd("B")
	list.AddToEnd("C")

	nodeB := list.FindNodeByValue("B")
	require.NotNil(t, nodeB)

	list.AddBeforeNode(nodeB, "A")
	assert.Equal(t, "A", list.Head.Key)
	assert.Equal(t, "B", list.Head.Next.Key)
	assert.Equal(t, "A", list.Head.Next.Prev.Key)

	list.AddBeforeNode(list.Head, "start")
	assert.Equal(t, "start", list.Head.Key)

	list.AddBeforeNode(nil, "test")
}

func TestDoubleList_JSONSerialization(t *testing.T) {
	list := NewDoubleList()
	list.Name = "test-list"
	list.AddToEnd("first")
	list.AddToEnd("second")
	list.AddToEnd("third")

	data, err := json.Marshal(list)
	require.NoError(t, err)
	assert.NotEmpty(t, data)

	var newList DoubleList
	err = json.Unmarshal(data, &newList)
	require.NoError(t, err)

	assert.Equal(t, list.Name, newList.Name)
	assert.Equal(t, list.GetAllElements(), newList.GetAllElements())
}

func TestDoubleList_BinarySerialization(t *testing.T) {
	list := NewDoubleList()
	list.Name = "test-list"
	list.AddToEnd("first")
	list.AddToEnd("second")
	list.AddToEnd("third")

	binaryData := list.SerializeBinary()
	assert.NotEmpty(t, binaryData)

	newList := NewDoubleList()
	err := newList.DeserializeBinary(binaryData)
	require.NoError(t, err)

	assert.Equal(t, list.Name, newList.Name)
	assert.Equal(t, list.GetAllElements(), newList.GetAllElements())
}

func TestDoubleList_Print(t *testing.T) {
	list := NewDoubleList()
	list.Name = "test-list"
	
	list.Print()

	list.AddToEnd("A")
	list.AddToEnd("B")
	list.AddToEnd("C")
	list.Print()
}
