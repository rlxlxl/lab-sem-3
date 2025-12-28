package main

import (
	"encoding/json"
	"os"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestDatabase_New(t *testing.T) {
	db := NewDatabase()
	assert.NotNil(t, db)
	assert.NotNil(t, db.Arrays)
	assert.NotNil(t, db.ForwardLists)
	assert.NotNil(t, db.DoubleLists)
	assert.NotNil(t, db.Stacks)
	assert.NotNil(t, db.Queues)
	assert.NotNil(t, db.Trees)
	assert.NotNil(t, db.ChainTables)
	assert.NotNil(t, db.OpenTables)
	assert.Equal(t, 0, len(db.Arrays))
}

func TestDatabase_FindMethods(t *testing.T) {
	db := NewDatabase()

	arr := NewDynamicArray(5)
	arr.Name = "test-array"
	db.Arrays = append(db.Arrays, arr)

	foundArr := db.findArray("test-array")
	assert.Equal(t, arr, foundArr)
	assert.Nil(t, db.findArray("nonexistent"))

	list := NewForwardList()
	list.Name = "test-list"
	db.ForwardLists = append(db.ForwardLists, list)

	foundList := db.findForwardList("test-list")
	assert.Equal(t, list, foundList)
	assert.Nil(t, db.findForwardList("nonexistent"))

	dlist := NewDoubleList()
	dlist.Name = "test-dlist"
	db.DoubleLists = append(db.DoubleLists, dlist)

	foundDList := db.findDoubleList("test-dlist")
	assert.Equal(t, dlist, foundDList)
	assert.Nil(t, db.findDoubleList("nonexistent"))

	stack := NewStack()
	stack.Name = "test-stack"
	db.Stacks = append(db.Stacks, stack)

	foundStack := db.findStack("test-stack")
	assert.Equal(t, stack, foundStack)
	assert.Nil(t, db.findStack("nonexistent"))

	queue := NewQueue()
	queue.Name = "test-queue"
	db.Queues = append(db.Queues, queue)

	foundQueue := db.findQueue("test-queue")
	assert.Equal(t, queue, foundQueue)
	assert.Nil(t, db.findQueue("nonexistent"))

	tree := NewRBTree()
	tree.Name = "test-tree"
	db.Trees = append(db.Trees, tree)

	foundTree := db.findTree("test-tree")
	assert.Equal(t, tree, foundTree)
	assert.Nil(t, db.findTree("nonexistent"))

	chainTable := NewChainHashTable(5)
	chainTable.Name = "test-chain"
	db.ChainTables = append(db.ChainTables, chainTable)

	foundChainTable := db.findChainTable("test-chain")
	assert.Equal(t, chainTable, foundChainTable)
	assert.Nil(t, db.findChainTable("nonexistent"))

	openTable := NewOpenHashTable(5)
	openTable.Name = "test-open"
	db.OpenTables = append(db.OpenTables, openTable)

	foundOpenTable := db.findOpenTable("test-open")
	assert.Equal(t, openTable, foundOpenTable)
	assert.Nil(t, db.findOpenTable("nonexistent"))
}

func TestDatabase_ProcessCommand_Array(t *testing.T) {
	db := NewDatabase()

	db.ProcessCommand("MPUSHTOEND arr1 value1")
	assert.Equal(t, 1, len(db.Arrays))
	assert.Equal(t, "arr1", db.Arrays[0].Name)

	db.ProcessCommand("MPUSHTOEND arr1 value2")
	assert.Equal(t, 2, db.Arrays[0].GetLength())

	db.ProcessCommand("MPUSHTOINDEX arr1 1 middle")
	val, _ := db.Arrays[0].GetElement(1)
	assert.Equal(t, "middle", val)

	db.ProcessCommand("MGET arr1 0")

	db.ProcessCommand("MDEL arr1 1")
	assert.Equal(t, 2, db.Arrays[0].GetLength())

	db.ProcessCommand("MSET arr1 0 newvalue")
	val, _ = db.Arrays[0].GetElement(0)
	assert.Equal(t, "newvalue", val)

	db.ProcessCommand("MLENGTH arr1")
}

func TestDatabase_ProcessCommand_ForwardList(t *testing.T) {
	db := NewDatabase()

	db.ProcessCommand("FPUSHHEAD flist1 value1")
	assert.Equal(t, 1, len(db.ForwardLists))
	assert.Equal(t, "flist1", db.ForwardLists[0].Name)

	db.ProcessCommand("FPUSHTAIL flist1 value2")
	assert.Equal(t, 2, len(db.ForwardLists[0].GetAllElements()))

	db.ProcessCommand("FDELVALUE flist1 value1")
	assert.Equal(t, 1, len(db.ForwardLists[0].GetAllElements()))

	db.ProcessCommand("FFIND flist1 value2")

	db.ProcessCommand("FGET flist1")
}

func TestDatabase_ProcessCommand_Stack(t *testing.T) {
	db := NewDatabase()

	db.ProcessCommand("SPUSH stack1 value1")
	assert.Equal(t, 1, len(db.Stacks))
	assert.Equal(t, "stack1", db.Stacks[0].Name)

	db.ProcessCommand("SPUSH stack1 value2")
	assert.Equal(t, 2, len(db.Stacks[0].GetAllElements()))

	db.ProcessCommand("SPOP stack1")
	assert.Equal(t, 1, len(db.Stacks[0].GetAllElements()))

	db.ProcessCommand("SGET stack1")
}

func TestDatabase_ProcessCommand_Queue(t *testing.T) {
	db := NewDatabase()

	db.ProcessCommand("QPUSH queue1 value1")
	assert.Equal(t, 1, len(db.Queues))
	assert.Equal(t, "queue1", db.Queues[0].Name)

	db.ProcessCommand("QPUSH queue1 value2")
	assert.Equal(t, 2, len(db.Queues[0].GetAllElements()))

	db.ProcessCommand("QPOP queue1")
	assert.Equal(t, 1, len(db.Queues[0].GetAllElements()))

	db.ProcessCommand("QGET queue1")
}

func TestDatabase_ProcessCommand_Tree(t *testing.T) {
	db := NewDatabase()

	db.ProcessCommand("TINSERT tree1 50")
	assert.Equal(t, 1, len(db.Trees))
	assert.Equal(t, "tree1", db.Trees[0].Name)

	db.ProcessCommand("TINSERT tree1 30")
	db.ProcessCommand("TINSERT tree1 70")
	assert.Equal(t, 3, len(db.Trees[0].GetAllElements()))

	db.ProcessCommand("TSEARCH tree1 30")
	db.ProcessCommand("TSEARCH tree1 100")
}

func TestDatabase_ProcessCommand_HashTables(t *testing.T) {
	db := NewDatabase()

	db.ProcessCommand("CHINSERT chtable1 value1")
	assert.Equal(t, 1, len(db.ChainTables))
	assert.Equal(t, "chtable1", db.ChainTables[0].Name)

	db.ProcessCommand("CHCONTAINS chtable1 value1")
	db.ProcessCommand("CHCONTAINS chtable1 value2")

	db.ProcessCommand("CHREMOVE chtable1 value1")

	db.ProcessCommand("CHPRINT chtable1")

	db.ProcessCommand("OHINSERT ohtable1 value1")
	assert.Equal(t, 1, len(db.OpenTables))
	assert.Equal(t, "ohtable1", db.OpenTables[0].Name)

	db.ProcessCommand("OHCONTAINS ohtable1 value1")

	db.ProcessCommand("OHREMOVE ohtable1 value1")

	db.ProcessCommand("OHPRINT ohtable1")
}

func TestDatabase_ProcessCommand_Invalid(t *testing.T) {
	db := NewDatabase()

	db.ProcessCommand("UNKNOWN_COMMAND")

	db.ProcessCommand("")
	db.ProcessCommand("PRINT")
}

func TestDatabase_TextSerialization(t *testing.T) {
	db := NewDatabase()

	db.ProcessCommand("MPUSHTOEND arr1 value1")
	db.ProcessCommand("MPUSHTOEND arr1 value2")
	
	db.ProcessCommand("FPUSHHEAD list1 item1")
	db.ProcessCommand("FPUSHTAIL list1 item2")
	
	db.ProcessCommand("SPUSH stack1 data1")
	db.ProcessCommand("QPUSH queue1 task1")
	
	db.ProcessCommand("TINSERT tree1 50")
	db.ProcessCommand("CHINSERT chtable1 hash1")
	db.ProcessCommand("OHINSERT ohtable1 open1")

	tmpfile, err := os.CreateTemp("", "testdb.*.json")
	require.NoError(t, err)
	defer os.Remove(tmpfile.Name())
	
	filename := tmpfile.Name()

	err = db.SaveToTextFile(filename)
	require.NoError(t, err)

	newDb := NewDatabase()

	err = newDb.LoadFromTextFile(filename)
	require.NoError(t, err)

	assert.Equal(t, len(db.Arrays), len(newDb.Arrays))
	assert.Equal(t, len(db.ForwardLists), len(newDb.ForwardLists))
	assert.Equal(t, len(db.Stacks), len(newDb.Stacks))
	assert.Equal(t, len(db.Queues), len(newDb.Queues))
	assert.Equal(t, len(db.Trees), len(newDb.Trees))
	assert.Equal(t, len(db.ChainTables), len(newDb.ChainTables))
	assert.Equal(t, len(db.OpenTables), len(newDb.OpenTables))
}

func TestDatabase_BinarySerialization(t *testing.T) {
	db := NewDatabase()

	db.ProcessCommand("MPUSHTOEND arr1 value1")
	db.ProcessCommand("MPUSHTOEND arr1 value2")
	
	db.ProcessCommand("FPUSHHEAD list1 item1")
	db.ProcessCommand("FPUSHTAIL list1 item2")

	tmpfile, err := os.CreateTemp("", "testdb.*.bin")
	require.NoError(t, err)
	defer os.Remove(tmpfile.Name())
	
	filename := tmpfile.Name()

	err = db.SaveToBinaryFile(filename)
	require.NoError(t, err)

	newDb := NewDatabase()

	err = newDb.LoadFromBinaryFile(filename)
	require.NoError(t, err)

	assert.Equal(t, len(db.Arrays), len(newDb.Arrays))
	assert.Equal(t, len(db.ForwardLists), len(newDb.ForwardLists))
}

func TestDatabase_PrintAll(t *testing.T) {
	db := NewDatabase()

	db.PrintAll()

	db.ProcessCommand("MPUSHTOEND arr1 value1")
	db.ProcessCommand("FPUSHHEAD list1 item1")
	db.PrintAll()
}

func TestDatabase_JSONSerialization(t *testing.T) {
	db := NewDatabase()
	db.ProcessCommand("MPUSHTOEND arr1 value1")
	db.ProcessCommand("FPUSHHEAD list1 item1")

	data, err := json.Marshal(db)
	require.NoError(t, err)
	assert.NotEmpty(t, data)

	var newDb Database
	err = json.Unmarshal(data, &newDb)
	require.NoError(t, err)

	assert.Equal(t, len(db.Arrays), len(newDb.Arrays))
	assert.Equal(t, len(db.ForwardLists), len(newDb.ForwardLists))
}

func TestDatabase_Help(t *testing.T) {
	db := NewDatabase()
	db.PrintHelp()
}
