package main

import (
	"encoding/json"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestRBTree_New(t *testing.T) {
	tree := NewRBTree()
	assert.NotNil(t, tree.Root)
	assert.NotNil(t, tree.Nil)
	assert.Equal(t, BLACK, tree.Root.Color)
	assert.Empty(t, tree.Name)
}

func TestRBTree_Insert(t *testing.T) {
	tree := NewRBTree()
	tree.Name = "test-tree"

	tree.Insert(50)
	assert.Equal(t, 50, tree.Root.Key)
	assert.Equal(t, BLACK, tree.Root.Color)

	tree.Insert(30)
	assert.True(t, tree.Search(30))

	tree.Insert(70)
	assert.True(t, tree.Search(70))

	tree.Insert(50)
	assert.True(t, tree.Search(50))

	tree.Insert(20)
	tree.Insert(40)
	tree.Insert(60)
	tree.Insert(80)

	assert.True(t, tree.Search(20))
	assert.True(t, tree.Search(40))
	assert.True(t, tree.Search(60))
	assert.True(t, tree.Search(80))
}

func TestRBTree_Search(t *testing.T) {
	tree := NewRBTree()
	tree.Name = "test-tree"

	assert.False(t, tree.Search(10))
	assert.False(t, tree.Search(20))

	tree.Insert(50)
	tree.Insert(30)
	tree.Insert(70)
	tree.Insert(20)
	tree.Insert(40)
	tree.Insert(60)
	tree.Insert(80)

	assert.True(t, tree.Search(50))
	assert.True(t, tree.Search(30))
	assert.True(t, tree.Search(70))
	assert.True(t, tree.Search(20))
	assert.True(t, tree.Search(40))
	assert.True(t, tree.Search(60))
	assert.True(t, tree.Search(80))

	assert.False(t, tree.Search(10))
	assert.False(t, tree.Search(90))
	assert.False(t, tree.Search(25))
}

func TestRBTree_GetAllElements(t *testing.T) {
	tree := NewRBTree()
	tree.Name = "test-tree"

	elements := tree.GetAllElements()
	assert.Empty(t, elements)

	tree.Insert(50)
	tree.Insert(30)
	tree.Insert(70)
	tree.Insert(20)
	tree.Insert(40)
	tree.Insert(60)
	tree.Insert(80)

	elements = tree.GetAllElements()
	expected := []int{20, 30, 40, 50, 60, 70, 80}
	assert.Equal(t, expected, elements)

	tree.Insert(10)
	tree.Insert(90)
	elements = tree.GetAllElements()
	expected = []int{10, 20, 30, 40, 50, 60, 70, 80, 90}
	assert.Equal(t, expected, elements)
}

func TestRBTree_SetAllElements(t *testing.T) {
	tree := NewRBTree()
	tree.Name = "test-tree"

	elements := []int{50, 30, 70, 20, 40, 60, 80}
	tree.SetAllElements(elements)

	assert.True(t, tree.Search(50))
	assert.True(t, tree.Search(30))
	assert.True(t, tree.Search(70))
	assert.True(t, tree.Search(20))
	assert.True(t, tree.Search(40))
	assert.True(t, tree.Search(60))
	assert.True(t, tree.Search(80))

	allElements := tree.GetAllElements()
	expected := []int{20, 30, 40, 50, 60, 70, 80}
	assert.Equal(t, expected, allElements)

	tree.SetAllElements([]int{})
	assert.False(t, tree.Search(50))
}

func TestRBTree_JSONSerialization(t *testing.T) {
	tree := NewRBTree()
	tree.Name = "test-tree"
	tree.Insert(50)
	tree.Insert(30)
	tree.Insert(70)
	tree.Insert(20)
	tree.Insert(40)

	data, err := json.Marshal(tree)
	require.NoError(t, err)
	assert.NotEmpty(t, data)

	var newTree RBTree
	err = json.Unmarshal(data, &newTree)
	require.NoError(t, err)

	assert.Equal(t, tree.Name, newTree.Name)
	assert.Equal(t, tree.GetAllElements(), newTree.GetAllElements())
}

func TestRBTree_BinarySerialization(t *testing.T) {
	tree := NewRBTree()
	tree.Name = "test-tree"
	tree.Insert(50)
	tree.Insert(30)
	tree.Insert(70)
	tree.Insert(20)
	tree.Insert(40)

	binaryData := tree.SerializeBinary()
	assert.NotEmpty(t, binaryData)

	newTree := NewRBTree()
	err := newTree.DeserializeBinary(binaryData)
	require.NoError(t, err)

	assert.Equal(t, tree.Name, newTree.Name)
	assert.Equal(t, tree.GetAllElements(), newTree.GetAllElements())
}

func TestRBTree_Print(t *testing.T) {
	tree := NewRBTree()
	tree.Name = "test-tree"
	
	tree.Print()

	tree.Insert(50)
	tree.Insert(30)
	tree.Insert(70)
	tree.Insert(20)
	tree.Insert(40)
	tree.Insert(60)
	tree.Insert(80)
	tree.Print()
}

func TestRBTree_ColorProperties(t *testing.T) {
	tree := NewRBTree()
	tree.Name = "test-tree"

	values := []int{11, 2, 14, 1, 7, 15, 5, 8, 4}
	for _, val := range values {
		tree.Insert(val)
	}

	assert.Equal(t, BLACK, tree.Root.Color)

	for _, val := range values {
		assert.True(t, tree.Search(val))
	}

	elements := tree.GetAllElements()
	for i := 1; i < len(elements); i++ {
		assert.True(t, elements[i-1] < elements[i])
	}
}
