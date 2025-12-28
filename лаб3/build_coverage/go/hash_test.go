package main

import (
	"encoding/json"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestChainHashTable_New(t *testing.T) {
	tests := []struct {
		name     string
		size     int
		expected int
	}{
		{"positive size", 10, 10},
		{"zero size", 0, 10},
		{"negative size", -5, 10},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			table := NewChainHashTable(tt.size)
			assert.Equal(t, tt.expected, table.TableSize)
			assert.Equal(t, 0, table.ElementCount)
			assert.NotNil(t, table.Table)
			assert.Len(t, table.Table, tt.expected)
		})
	}
}

func TestChainHashTable_Insert(t *testing.T) {
	table := NewChainHashTable(5)
	table.Name = "test-table"

	table.Insert("first")
	assert.Equal(t, 1, table.ElementCount)
	assert.True(t, table.Contains("first"))

	table.Insert("second")
	assert.Equal(t, 2, table.ElementCount)
	assert.True(t, table.Contains("second"))

	table.Insert("first")
	assert.Equal(t, 2, table.ElementCount)

	table.Insert("third")
	table.Insert("fourth")
	table.Insert("fifth")
	assert.Equal(t, 5, table.ElementCount)
}

func TestChainHashTable_Contains(t *testing.T) {
	table := NewChainHashTable(5)
	table.Name = "test-table"

	assert.False(t, table.Contains("test"))

	table.Insert("apple")
	table.Insert("banana")
	table.Insert("cherry")

	assert.True(t, table.Contains("apple"))
	assert.True(t, table.Contains("banana"))
	assert.True(t, table.Contains("cherry"))
	assert.False(t, table.Contains("nonexistent"))

	table.Remove("banana")
	assert.False(t, table.Contains("banana"))
	assert.True(t, table.Contains("apple"))
}

func TestChainHashTable_Remove(t *testing.T) {
	table := NewChainHashTable(5)
	table.Name = "test-table"

	removed := table.Remove("test")
	assert.False(t, removed)

	table.Insert("A")
	table.Insert("B")
	table.Insert("C")

	removed = table.Remove("B")
	assert.True(t, removed)
	assert.Equal(t, 2, table.ElementCount)
	assert.False(t, table.Contains("B"))

	removed = table.Remove("nonexistent")
	assert.False(t, removed)
	assert.Equal(t, 2, table.ElementCount)

	table.Remove("A")
	table.Remove("C")
	assert.Equal(t, 0, table.ElementCount)
}

func TestChainHashTable_GetAllElements(t *testing.T) {
	table := NewChainHashTable(5)
	table.Name = "test-table"

	elements := table.GetAllElements()
	assert.Empty(t, elements)

	items := []string{"apple", "banana", "cherry", "date", "elderberry"}
	for _, item := range items {
		table.Insert(item)
	}

	elements = table.GetAllElements()
	assert.Equal(t, len(items), len(elements))
	
	for _, item := range items {
		assert.Contains(t, elements, item)
	}

	table.Remove("banana")
	elements = table.GetAllElements()
	assert.Equal(t, len(items)-1, len(elements))
	assert.NotContains(t, elements, "banana")
}

func TestChainHashTable_JSONSerialization(t *testing.T) {
	table := NewChainHashTable(5)
	table.Name = "test-table"
	table.Insert("first")
	table.Insert("second")
	table.Insert("third")

	data, err := json.Marshal(table)
	require.NoError(t, err)
	assert.NotEmpty(t, data)

	var newTable ChainHashTable
	err = json.Unmarshal(data, &newTable)
	require.NoError(t, err)

	assert.Equal(t, table.Name, newTable.Name)
	assert.Equal(t, table.TableSize, newTable.TableSize)
	assert.Equal(t, table.ElementCount, newTable.ElementCount)
	
	for _, elem := range table.GetAllElements() {
		assert.True(t, newTable.Contains(elem))
	}
}

func TestChainHashTable_BinarySerialization(t *testing.T) {
	table := NewChainHashTable(5)
	table.Name = "test-table"
	table.Insert("first")
	table.Insert("second")
	table.Insert("third")

	binaryData := table.SerializeBinary()
	assert.NotEmpty(t, binaryData)

	newTable := NewChainHashTable(1)
	err := newTable.DeserializeBinary(binaryData)
	require.NoError(t, err)

	assert.Equal(t, table.Name, newTable.Name)
	assert.Equal(t, table.TableSize, newTable.TableSize)
	assert.Equal(t, table.ElementCount, newTable.ElementCount)
	
	for _, elem := range table.GetAllElements() {
		assert.True(t, newTable.Contains(elem))
	}
}

func TestOpenHashTable_New(t *testing.T) {
	tests := []struct {
		name     string
		size     int
		expected int
	}{
		{"positive size", 10, 10},
		{"zero size", 0, 10},
		{"negative size", -5, 10},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			table := NewOpenHashTable(tt.size)
			assert.Equal(t, tt.expected, table.TableSize)
			assert.Equal(t, 0, table.ElementCount)
			assert.NotNil(t, table.Table)
			assert.Len(t, table.Table, tt.expected)
		})
	}
}

func TestOpenHashTable_Insert(t *testing.T) {
	table := NewOpenHashTable(5)
	table.Name = "test-table"

	table.Insert("first")
	assert.Equal(t, 1, table.ElementCount)
	assert.True(t, table.Contains("first"))

	table.Insert("second")
	assert.Equal(t, 2, table.ElementCount)
	assert.True(t, table.Contains("second"))

	table.Insert("first")
	assert.Equal(t, 2, table.ElementCount)

	table.Insert("third")
	table.Insert("fourth")
	table.Insert("fifth")
	assert.Equal(t, 5, table.ElementCount)
}

func TestOpenHashTable_Contains(t *testing.T) {
	table := NewOpenHashTable(5)
	table.Name = "test-table"

	assert.False(t, table.Contains("test"))

	table.Insert("apple")
	table.Insert("banana")
	table.Insert("cherry")

	assert.True(t, table.Contains("apple"))
	assert.True(t, table.Contains("banana"))
	assert.True(t, table.Contains("cherry"))
	assert.False(t, table.Contains("nonexistent"))

	table.Remove("banana")
	assert.False(t, table.Contains("banana"))
	assert.True(t, table.Contains("apple"))
}

func TestOpenHashTable_Remove(t *testing.T) {
	table := NewOpenHashTable(5)
	table.Name = "test-table"

	removed := table.Remove("test")
	assert.False(t, removed)

	table.Insert("A")
	table.Insert("B")
	table.Insert("C")

	removed = table.Remove("B")
	assert.True(t, removed)
	assert.Equal(t, 2, table.ElementCount)
	assert.False(t, table.Contains("B"))

	removed = table.Remove("nonexistent")
	assert.False(t, removed)
	assert.Equal(t, 2, table.ElementCount)

	table.Remove("A")
	table.Remove("C")
	assert.Equal(t, 0, table.ElementCount)
}

func TestOpenHashTable_GetAllElements(t *testing.T) {
	table := NewOpenHashTable(5)
	table.Name = "test-table"

	elements := table.GetAllElements()
	assert.Empty(t, elements)

	items := []string{"apple", "banana", "cherry"}
	for _, item := range items {
		table.Insert(item)
	}

	elements = table.GetAllElements()
	assert.Equal(t, len(items), len(elements))
	
	for _, item := range items {
		assert.Contains(t, elements, item)
	}

	table.Remove("banana")
	elements = table.GetAllElements()
	assert.Equal(t, len(items)-1, len(elements))
	assert.NotContains(t, elements, "banana")
}

func TestOpenHashTable_JSONSerialization(t *testing.T) {
	table := NewOpenHashTable(5)
	table.Name = "test-table"
	table.Insert("first")
	table.Insert("second")
	table.Insert("third")

	data, err := json.Marshal(table)
	require.NoError(t, err)
	assert.NotEmpty(t, data)

	var newTable OpenHashTable
	err = json.Unmarshal(data, &newTable)
	require.NoError(t, err)

	assert.Equal(t, table.Name, newTable.Name)
	assert.Equal(t, table.TableSize, newTable.TableSize)
	assert.Equal(t, table.ElementCount, newTable.ElementCount)
	
	for _, elem := range table.GetAllElements() {
		assert.True(t, newTable.Contains(elem))
	}
}

func TestOpenHashTable_BinarySerialization(t *testing.T) {
	table := NewOpenHashTable(5)
	table.Name = "test-table"
	table.Insert("first")
	table.Insert("second")
	table.Insert("third")

	binaryData := table.SerializeBinary()
	assert.NotEmpty(t, binaryData)

	newTable := NewOpenHashTable(1)
	err := newTable.DeserializeBinary(binaryData)
	require.NoError(t, err)

	assert.Equal(t, table.Name, newTable.Name)
	assert.Equal(t, table.TableSize, newTable.TableSize)
	assert.Equal(t, table.ElementCount, newTable.ElementCount)
	
	for _, elem := range table.GetAllElements() {
		assert.True(t, newTable.Contains(elem))
	}
}

func TestHashTables_Print(t *testing.T) {
	chainTable := NewChainHashTable(3)
	chainTable.Name = "chain-test"
	chainTable.Insert("A")
	chainTable.Insert("B")
	chainTable.Print()

	openTable := NewOpenHashTable(3)
	openTable.Name = "open-test"
	openTable.Insert("X")
	openTable.Insert("Y")
	openTable.Print()
}
