package main

import (
	"encoding/json"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestForwardList_New(t *testing.T) {
	list := NewForwardList()
	assert.Nil(t, list.Head)
	assert.Empty(t, list.Name)
}

func TestForwardList_AddToEnd(t *testing.T) {
	list := NewForwardList()
	list.Name = "test-list"

	list.AddToEnd("first")
	assert.NotNil(t, list.Head)
	assert.Equal(t, "first", list.Head.Key)

	list.AddToEnd("second")
	assert.Equal(t, "first", list.Head.Key)
	assert.Equal(t, "second", list.Head.Next.Key)

	list.AddToEnd("third")
	assert.Equal(t, "third", list.Head.Next.Next.Key)
}

func TestForwardList_AddToBeginning(t *testing.T) {
	list := NewForwardList()
	list.Name = "test-list"

	list.AddToBeginning("first")
	assert.Equal(t, "first", list.Head.Key)

	list.AddToBeginning("second")
	assert.Equal(t, "second", list.Head.Key)
	assert.Equal(t, "first", list.Head.Next.Key)

	list.AddToBeginning("third")
	assert.Equal(t, "third", list.Head.Key)
	assert.Equal(t, "second", list.Head.Next.Key)
}

func TestForwardList_AddAfterNode(t *testing.T) {
	list := NewForwardList()
	list.Name = "test-list"

	list.AddToEnd("A")
	list.AddToEnd("B")
	list.AddToEnd("D")

	nodeB := list.FindNodeByValue("B")
	require.NotNil(t, nodeB)

	list.AddAfterNode(nodeB, "C")
	assert.Equal(t, "C", nodeB.Next.Key)
	assert.Equal(t, "D", nodeB.Next.Next.Key)

	list.AddAfterNode(nil, "test")
}

func TestForwardList_AddBeforeNode(t *testing.T) {
	list := NewForwardList()
	list.Name = "test-list"

	list.AddToEnd("A")
	list.AddToEnd("C")
	list.AddToEnd("D")

	nodeC := list.FindNodeByValue("C")
	require.NotNil(t, nodeC)

	list.AddBeforeNode(nodeC, "B")
	assert.Equal(t, "B", list.Head.Next.Key)

	list.AddBeforeNode(list.Head, "start")
	assert.Equal(t, "start", list.Head.Key)

	list.AddBeforeNode(nil, "test")
}

func TestForwardList_GetFirst(t *testing.T) {
	list := NewForwardList()
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

func TestForwardList_DeleteByValue(t *testing.T) {
	list := NewForwardList()
	list.Name = "test-list"

	deleted := list.DeleteByValue("test")
	assert.False(t, deleted)

	list.AddToEnd("only")
	deleted = list.DeleteByValue("only")
	assert.True(t, deleted)
	assert.Nil(t, list.Head)

	list.AddToEnd("A")
	list.AddToEnd("B")
	list.AddToEnd("C")
	deleted = list.DeleteByValue("A")
	assert.True(t, deleted)
	assert.Equal(t, "B", list.Head.Key)

	list.AddToEnd("D")
	deleted = list.DeleteByValue("C")
	assert.True(t, deleted)
	assert.Equal(t, "D", list.Head.Next.Key)

	deleted = list.DeleteByValue("nonexistent")
	assert.False(t, deleted)
}

func TestForwardList_FindNodeByValue(t *testing.T) {
	list := NewForwardList()
	list.Name = "test-list"

	node := list.FindNodeByValue("test")
	assert.Nil(t, node)

	list.AddToEnd("A")
	list.AddToEnd("B")
	list.AddToEnd("C")

	node = list.FindNodeByValue("B")
	require.NotNil(t, node)
	assert.Equal(t, "B", node.Key)
	assert.Equal(t, "C", node.Next.Key)

	node = list.FindNodeByValue("C")
	require.NotNil(t, node)
	assert.Equal(t, "C", node.Key)
	assert.Nil(t, node.Next)

	node = list.FindNodeByValue("nonexistent")
	assert.Nil(t, node)
}

func TestForwardList_GetAllElements(t *testing.T) {
	list := NewForwardList()
	list.Name = "test-list"

	elements := list.GetAllElements()
	assert.Empty(t, elements)

	list.AddToEnd("A")
	list.AddToEnd("B")
	list.AddToEnd("C")

	elements = list.GetAllElements()
	assert.Equal(t, []string{"A", "B", "C"}, elements)

	list.DeleteByValue("B")
	elements = list.GetAllElements()
	assert.Equal(t, []string{"A", "C"}, elements)
}

func TestForwardList_SetAllElements(t *testing.T) {
	list := NewForwardList()
	list.Name = "test-list"

	elements := []string{"X", "Y", "Z"}
	list.SetAllElements(elements)

	assert.Equal(t, "X", list.Head.Key)
	assert.Equal(t, "Y", list.Head.Next.Key)
	assert.Equal(t, "Z", list.Head.Next.Next.Key)

	list.SetAllElements([]string{})
	assert.Nil(t, list.Head)
}

func TestForwardList_JSONSerialization(t *testing.T) {
	list := NewForwardList()
	list.Name = "test-list"
	list.AddToEnd("first")
	list.AddToEnd("second")
	list.AddToEnd("third")

	data, err := json.Marshal(list)
	require.NoError(t, err)
	assert.NotEmpty(t, data)

	var newList ForwardList
	err = json.Unmarshal(data, &newList)
	require.NoError(t, err)

	assert.Equal(t, list.Name, newList.Name)
	assert.Equal(t, list.GetAllElements(), newList.GetAllElements())
}

func TestForwardList_BinarySerialization(t *testing.T) {
	list := NewForwardList()
	list.Name = "test-list"
	list.AddToEnd("first")
	list.AddToEnd("second")
	list.AddToEnd("third")

	binaryData := list.SerializeBinary()
	assert.NotEmpty(t, binaryData)

	newList := NewForwardList()
	err := newList.DeserializeBinary(binaryData)
	require.NoError(t, err)

	assert.Equal(t, list.Name, newList.Name)
	assert.Equal(t, list.GetAllElements(), newList.GetAllElements())
}

func TestForwardList_Print(t *testing.T) {
	list := NewForwardList()
	list.Name = "test-list"
	
	list.Print()

	list.AddToEnd("A")
	list.AddToEnd("B")
	list.AddToEnd("C")
	list.Print()
}
