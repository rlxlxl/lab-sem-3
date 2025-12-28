package main

import (
	"encoding/json"
	"fmt"
	"os"
	"strconv"
	"strings"
)

const MAX_STRUCTURES = 100

type Database struct {
	Arrays       []*DynamicArray   `json:"arrays"`
	ForwardLists []*ForwardList    `json:"forward_lists"`
	DoubleLists  []*DoubleList     `json:"double_lists"`
	Stacks       []*Stack          `json:"stacks"`
	Queues       []*Queue          `json:"queues"`
	Trees        []*RBTree         `json:"trees"`
	ChainTables  []*ChainHashTable `json:"chain_tables"`
	OpenTables   []*OpenHashTable  `json:"open_tables"`
}

func NewDatabase() *Database {
	return &Database{
		Arrays:       make([]*DynamicArray, 0, MAX_STRUCTURES),
		ForwardLists: make([]*ForwardList, 0, MAX_STRUCTURES),
		DoubleLists:  make([]*DoubleList, 0, MAX_STRUCTURES),
		Stacks:       make([]*Stack, 0, MAX_STRUCTURES),
		Queues:       make([]*Queue, 0, MAX_STRUCTURES),
		Trees:        make([]*RBTree, 0, MAX_STRUCTURES),
		ChainTables:  make([]*ChainHashTable, 0, MAX_STRUCTURES),
		OpenTables:   make([]*OpenHashTable, 0, MAX_STRUCTURES),
	}
}

func (db *Database) ProcessCommand(command string) {
	parts := strings.Fields(command)
	if len(parts) == 0 {
		return
	}
	
	cmd := parts[0]
	
	switch cmd {
        case "MPUSHTOEND":
		if len(parts) >= 3 {
			name := parts[1]
			value := parts[2]
			arr := db.findArray(name)
			if arr == nil {
				if len(db.Arrays) < MAX_STRUCTURES {
					arr = NewDynamicArray(10)
					arr.Name = name
					db.Arrays = append(db.Arrays, arr)
					fmt.Printf("Создан новый массив: %s\n", name)
				}
			}
			if arr != nil {
				arr.AddToEnd(value)
				fmt.Println(value)
			}
		}
	
	case "MPUSHTOINDEX":
		if len(parts) >= 4 {
			name := parts[1]
			index, err := strconv.Atoi(parts[2])
			if err != nil {
				fmt.Println("Ошибка: неверный индекс")
				return
			}
			value := parts[3]
			arr := db.findArray(name)
			if arr == nil {
				if len(db.Arrays) < MAX_STRUCTURES {
					arr = NewDynamicArray(10)
					arr.Name = name
					db.Arrays = append(db.Arrays, arr)
					fmt.Printf("Создан новый массив: %s\n", name)
				}
			}
			if arr != nil {
				if err := arr.AddAtIndex(index, value); err != nil {
					fmt.Printf("Ошибка: %v\n", err)
				} else {
					fmt.Println(value)
				}
			}
		}
	
	case "MGET":
		if len(parts) >= 3 {
			name := parts[1]
			index, err := strconv.Atoi(parts[2])
			if err != nil {
				fmt.Println("Ошибка: неверный индекс")
				return
			}
			arr := db.findArray(name)
			if arr != nil {
				value, err := arr.GetElement(index)
				if err != nil {
					fmt.Printf("Ошибка: %v\n", err)
				} else {
					fmt.Println(value)
				}
			} else {
				fmt.Printf("Массив %s не найден\n", name)
			}
		}
	
	case "MDEL":
		if len(parts) >= 3 {
			name := parts[1]
			index, err := strconv.Atoi(parts[2])
			if err != nil {
				fmt.Println("Ошибка: неверный индекс")
				return
			}
			arr := db.findArray(name)
			if arr != nil {
				if err := arr.RemoveAtIndex(index); err != nil {
					fmt.Printf("Ошибка: %v\n", err)
				} else {
					fmt.Printf("Удален элемент по индексу %d из массива %s\n", index, name)
				}
			} else {
				fmt.Printf("Массив %s не найден\n", name)
			}
		}
	
	case "MSET":
		if len(parts) >= 4 {
			name := parts[1]
			index, err := strconv.Atoi(parts[2])
			if err != nil {
				fmt.Println("Ошибка: неверный индекс")
				return
			}
			value := parts[3]
			arr := db.findArray(name)
			if arr != nil {
				if err := arr.SetElement(index, value); err != nil {
					fmt.Printf("Ошибка: %v\n", err)
				} else {
					fmt.Printf("Заменен элемент по индексу %d в массиве %s на значение %s\n", index, name, value)
				}
			} else {
				fmt.Printf("Массив %s не найден\n", name)
			}
		}
	
	case "MLENGTH":
		if len(parts) >= 2 {
			name := parts[1]
			arr := db.findArray(name)
			if arr != nil {
				fmt.Println(arr.GetLength())
			} else {
				fmt.Printf("Массив %s не найден\n", name)
			}
		}
	
	// ---------- Односвязный список ----------
	case "FPUSHHEAD":
		if len(parts) >= 3 {
			name := parts[1]
			value := parts[2]
			list := db.findForwardList(name)
			if list == nil {
				if len(db.ForwardLists) < MAX_STRUCTURES {
					list = NewForwardList()
					list.Name = name
					list.AddToBeginning(value)
					db.ForwardLists = append(db.ForwardLists, list)
					fmt.Printf("Создан новый односвязный список: %s\n", name)
				}
			} else {
				list.AddToBeginning(value)
			}
			if list != nil {
				fmt.Println(value)
			}
		}
	
	case "FPUSHTAIL":
		if len(parts) >= 3 {
			name := parts[1]
			value := parts[2]
			list := db.findForwardList(name)
			if list == nil {
				if len(db.ForwardLists) < MAX_STRUCTURES {
					list = NewForwardList()
					list.Name = name
					list.AddToEnd(value)
					db.ForwardLists = append(db.ForwardLists, list)
					fmt.Printf("Создан новый односвязный список: %s\n", name)
				}
			} else {
				list.AddToEnd(value)
			}
			if list != nil {
				fmt.Println(value)
			}
		}
	
	case "FDELVALUE":
		if len(parts) >= 3 {
			name := parts[1]
			value := parts[2]
			list := db.findForwardList(name)
			if list != nil {
				if list.DeleteByValue(value) {
					fmt.Printf("Удален элемент %s из односвязного списка %s\n", value, name)
				} else {
					fmt.Printf("Элемент %s не найден в односвязном списке %s\n", value, name)
				}
			} else {
				fmt.Printf("Односвязный список %s не найден\n", name)
			}
		}
	
	case "FPUSHAFTER":
		if len(parts) >= 4 {
			name := parts[1]
			targetValue := parts[2]
			value := parts[3]
			list := db.findForwardList(name)
			if list != nil {
				targetNode := list.FindNodeByValue(targetValue)
				if targetNode != nil {
					list.AddAfterNode(targetNode, value)
					fmt.Println(value)
				} else {
					fmt.Printf("Элемент %s не найден в односвязном списке %s\n", targetValue, name)
				}
			} else {
				fmt.Printf("Односвязный список %s не найден\n", name)
			}
		}
	
	case "FPUSHBEFORE":
		if len(parts) >= 4 {
			name := parts[1]
			targetValue := parts[2]
			value := parts[3]
			list := db.findForwardList(name)
			if list != nil {
				targetNode := list.FindNodeByValue(targetValue)
				if targetNode != nil {
					list.AddBeforeNode(targetNode, value)
					fmt.Println(value)
				} else {
					fmt.Printf("Элемент %s не найден в односвязном списке %s\n", targetValue, name)
				}
			} else {
				fmt.Printf("Односвязный список %s не найден\n", name)
			}
		}
	
	case "FFIND":
		if len(parts) >= 3 {
			name := parts[1]
			value := parts[2]
			list := db.findForwardList(name)
			if list != nil {
				found := list.FindNodeByValue(value)
				if found != nil {
					fmt.Println("TRUE")
				} else {
					fmt.Println("FALSE")
				}
			} else {
				fmt.Printf("Односвязный список %s не найден\n", name)
			}
		}
	
	case "FGET":
		if len(parts) >= 2 {
			name := parts[1]
			list := db.findForwardList(name)
			if list != nil {
				value, err := list.GetFirst()
				if err != nil {
					fmt.Printf("Ошибка: %v\n", err)
				} else {
					fmt.Println(value)
				}
			} else {
				fmt.Printf("Односвязный список %s не найден\n", name)
			}
		}
	
	// ---------- Двусвязный список ----------
	case "LPUSHHEAD":
		if len(parts) >= 3 {
			name := parts[1]
			value := parts[2]
			list := db.findDoubleList(name)
			if list == nil {
				if len(db.DoubleLists) < MAX_STRUCTURES {
					list = NewDoubleList()
					list.Name = name
					list.AddToBeginning(value)
					db.DoubleLists = append(db.DoubleLists, list)
					fmt.Printf("Создан новый двусвязный список: %s\n", name)
				}
			} else {
				list.AddToBeginning(value)
			}
			if list != nil {
				fmt.Println(value)
			}
		}
	
	case "LPUSHTAIL":
		if len(parts) >= 3 {
			name := parts[1]
			value := parts[2]
			list := db.findDoubleList(name)
			if list == nil {
				if len(db.DoubleLists) < MAX_STRUCTURES {
					list = NewDoubleList()
					list.Name = name
					list.AddToEnd(value)
					db.DoubleLists = append(db.DoubleLists, list)
					fmt.Printf("Создан новый двусвязный список: %s\n", name)
				}
			} else {
				list.AddToEnd(value)
			}
			if list != nil {
				fmt.Println(value)
			}
		}
	
	case "LDELVALUE":
		if len(parts) >= 3 {
			name := parts[1]
			value := parts[2]
			list := db.findDoubleList(name)
			if list != nil {
				if list.DeleteByValue(value) {
					fmt.Printf("Удален элемент %s из двусвязного списка %s\n", value, name)
				} else {
					fmt.Printf("Элемент %s не найден в двусвязном списке %s\n", value, name)
				}
			} else {
				fmt.Printf("Двусвязный список %s не найден\n", name)
			}
		}
	
	case "LFIND":
		if len(parts) >= 3 {
			name := parts[1]
			value := parts[2]
			list := db.findDoubleList(name)
			if list != nil {
				found := list.FindNodeByValue(value)
				if found != nil {
					fmt.Println("TRUE")
				} else {
					fmt.Println("FALSE")
				}
			} else {
				fmt.Printf("Двусвязный список %s не найден\n", name)
			}
		}
	
	case "LGET":
		if len(parts) >= 2 {
			name := parts[1]
			list := db.findDoubleList(name)
			if list != nil {
				value, err := list.GetFirst()
				if err != nil {
					fmt.Printf("Ошибка: %v\n", err)
				} else {
					fmt.Println(value)
				}
			} else {
				fmt.Printf("Двусвязный список %s не найден\n", name)
			}
		}
	
	// ---------- Стек ----------
	case "SPUSH":
		if len(parts) >= 3 {
			name := parts[1]
			value := parts[2]
			stack := db.findStack(name)
			if stack == nil {
				if len(db.Stacks) < MAX_STRUCTURES {
					stack = NewStack()
					stack.Name = name
					db.Stacks = append(db.Stacks, stack)
					fmt.Printf("Создан новый стек: %s\n", name)
				}
			}
			if stack != nil {
				stack.Push(value)
				fmt.Println(value)
			}
		}
	
	case "SPOP":
		if len(parts) >= 2 {
			name := parts[1]
			stack := db.findStack(name)
			if stack != nil {
				value, err := stack.Pop()
				if err != nil {
					fmt.Printf("Ошибка: %v\n", err)
				} else {
					fmt.Println(value)
				}
			} else {
				fmt.Printf("Стек %s не найден\n", name)
			}
		}
	
	case "SGET":
		if len(parts) >= 2 {
			name := parts[1]
			stack := db.findStack(name)
			if stack != nil {
				value, err := stack.Peek()
				if err != nil {
					fmt.Printf("Ошибка: %v\n", err)
				} else {
					fmt.Println(value)
				}
			} else {
				fmt.Printf("Стек %s не найден\n", name)
			}
		}
	
	// ---------- Очередь ----------
	case "QPUSH":
		if len(parts) >= 3 {
			name := parts[1]
			value := parts[2]
			queue := db.findQueue(name)
			if queue == nil {
				if len(db.Queues) < MAX_STRUCTURES {
					queue = NewQueue()
					queue.Name = name
					db.Queues = append(db.Queues, queue)
					fmt.Printf("Создана новая очередь: %s\n", name)
				}
			}
			if queue != nil {
				queue.Enqueue(value)
				fmt.Println(value)
			}
		}
	
	case "QPOP":
		if len(parts) >= 2 {
			name := parts[1]
			queue := db.findQueue(name)
			if queue != nil {
				value, err := queue.Dequeue()
				if err != nil {
					fmt.Printf("Ошибка: %v\n", err)
				} else {
					fmt.Println(value)
				}
			} else {
				fmt.Printf("Очередь %s не найдена\n", name)
			}
		}
	
	case "QGET":
		if len(parts) >= 2 {
			name := parts[1]
			queue := db.findQueue(name)
			if queue != nil {
				value, err := queue.Peek()
				if err != nil {
					fmt.Printf("Ошибка: %v\n", err)
				} else {
					fmt.Println(value)
				}
			} else {
				fmt.Printf("Очередь %s не найдена\n", name)
			}
		}
	
	// ---------- Красно-черное дерево ----------
	case "TINSERT":
		if len(parts) >= 3 {
			name := parts[1]
			valueStr := parts[2]
			value, err := strconv.Atoi(valueStr)
			if err != nil {
				fmt.Println("Ошибка: неверное значение")
				return
			}
			tree := db.findTree(name)
			if tree == nil {
				if len(db.Trees) < MAX_STRUCTURES {
					tree = NewRBTree()
					tree.Name = name
					db.Trees = append(db.Trees, tree)
					fmt.Printf("Создано новое красно-черное дерево: %s\n", name)
				}
			}
			if tree != nil {
				tree.Insert(value)
				fmt.Println(value)
			}
		}
	
	case "TSEARCH":
		if len(parts) >= 3 {
			name := parts[1]
			valueStr := parts[2]
			value, err := strconv.Atoi(valueStr)
			if err != nil {
				fmt.Println("Ошибка: неверное значение")
				return
			}
			tree := db.findTree(name)
			if tree != nil {
				if tree.Search(value) {
					fmt.Println("TRUE")
				} else {
					fmt.Println("FALSE")
				}
			} else {
				fmt.Printf("Дерево %s не найдено\n", name)
			}
		}
	
	// ---------- Хэш-таблица (цепочки) ----------
	case "CHINSERT":
		if len(parts) >= 3 {
			name := parts[1]
			value := parts[2]
			table := db.findChainTable(name)
			if table == nil {
				if len(db.ChainTables) < MAX_STRUCTURES {
					table = NewChainHashTable(10)
					table.Name = name
					db.ChainTables = append(db.ChainTables, table)
					fmt.Printf("Создана новая хэш-таблица (метод цепочек): %s\n", name)
				}
			}
			if table != nil {
				table.Insert(value)
				fmt.Println(value)
			}
		}
	
	case "CHCONTAINS":
		if len(parts) >= 3 {
			name := parts[1]
			value := parts[2]
			table := db.findChainTable(name)
			if table != nil {
				if table.Contains(value) {
					fmt.Println("TRUE")
				} else {
					fmt.Println("FALSE")
				}
			} else {
				fmt.Printf("Хэш-таблица %s не найдена\n", name)
			}
		}
	
	case "CHREMOVE":
		if len(parts) >= 3 {
			name := parts[1]
			value := parts[2]
			table := db.findChainTable(name)
			if table != nil {
				if table.Remove(value) {
					fmt.Printf("Удален элемент %s из хэш-таблицы %s\n", value, name)
				} else {
					fmt.Printf("Элемент %s не найден в хэш-таблице %s\n", value, name)
				}
			} else {
				fmt.Printf("Хэш-таблица %s не найдена\n", name)
			}
		}
	
	case "CHPRINT":
		if len(parts) >= 2 {
			name := parts[1]
			table := db.findChainTable(name)
			if table != nil {
				table.Print()
			} else {
				fmt.Printf("Хэш-таблица %s не найдена\n", name)
			}
		}
	
	// ---------- Хэш-таблица (открытая адресация) ----------
	case "OHINSERT":
		if len(parts) >= 3 {
			name := parts[1]
			value := parts[2]
			table := db.findOpenTable(name)
			if table == nil {
				if len(db.OpenTables) < MAX_STRUCTURES {
					table = NewOpenHashTable(10)
					table.Name = name
					db.OpenTables = append(db.OpenTables, table)
					fmt.Printf("Создана новая хэш-таблица (открытая адресация): %s\n", name)
				}
			}
			if table != nil {
				table.Insert(value)
				fmt.Println(value)
			}
		}
	
	case "OHCONTAINS":
		if len(parts) >= 3 {
			name := parts[1]
			value := parts[2]
			table := db.findOpenTable(name)
			if table != nil {
				if table.Contains(value) {
					fmt.Println("TRUE")
				} else {
					fmt.Println("FALSE")
				}
			} else {
				fmt.Printf("Хэш-таблица %s не найдена\n", name)
			}
		}
	
	case "OHREMOVE":
		if len(parts) >= 3 {
			name := parts[1]
			value := parts[2]
			table := db.findOpenTable(name)
			if table != nil {
				if table.Remove(value) {
					fmt.Printf("Удален элемент %s из хэш-таблицы %s\n", value, name)
				} else {
					fmt.Printf("Элемент %s не найден в хэш-таблице %s\n", value, name)
				}
			} else {
				fmt.Printf("Хэш-таблица %s не найдена\n", name)
			}
		}
	
	case "OHPRINT":
		if len(parts) >= 2 {
			name := parts[1]
			table := db.findOpenTable(name)
			if table != nil {
				table.Print()
			} else {
				fmt.Printf("Хэш-таблица %s не найдена\n", name)
			}
		}
	
	case "PRINT":
		db.PrintAll()
	
	case "SAVE_TEXT":
		if len(parts) >= 2 {
			filename := parts[1]
			if err := db.SaveToTextFile(filename); err != nil {
				fmt.Printf("Ошибка сохранения: %v\n", err)
			} else {
				fmt.Printf("Данные сохранены в текстовый файл: %s\n", filename)
			}
		}
	
	case "LOAD_TEXT":
		if len(parts) >= 2 {
			filename := parts[1]
			if err := db.LoadFromTextFile(filename); err != nil {
				fmt.Printf("Ошибка загрузки: %v\n", err)
			} else {
				fmt.Printf("Данные загружены из текстового файла: %s\n", filename)
			}
		}
	
	case "SAVE_BINARY":
		if len(parts) >= 2 {
			filename := parts[1]
			if err := db.SaveToBinaryFile(filename); err != nil {
				fmt.Printf("Ошибка сохранения: %v\n", err)
			} else {
				fmt.Printf("Данные сохранены в бинарный файл: %s\n", filename)
			}
		}
	
	case "LOAD_BINARY":
		if len(parts) >= 2 {
			filename := parts[1]
			if err := db.LoadFromBinaryFile(filename); err != nil {
				fmt.Printf("Ошибка загрузки: %v\n", err)
			} else {
				fmt.Printf("Данные загружены из бинарного файла: %s\n", filename)
			}
		}
	
	case "help":
		db.PrintHelp()
	
	default:
		fmt.Printf("Неизвестная команда: %s\n", cmd)
	}
}

// Вспомогательные методы поиска
func (db *Database) findArray(name string) *DynamicArray {
	for _, arr := range db.Arrays {
		if arr.Name == name {
			return arr
		}
	}
	return nil
}

func (db *Database) findForwardList(name string) *ForwardList {
	for _, list := range db.ForwardLists {
		if list.Name == name {
			return list
		}
	}
	return nil
}

func (db *Database) findDoubleList(name string) *DoubleList {
	for _, list := range db.DoubleLists {
		if list.Name == name {
			return list
		}
	}
	return nil
}

func (db *Database) findStack(name string) *Stack {
	for _, stack := range db.Stacks {
		if stack.Name == name {
			return stack
		}
	}
	return nil
}

func (db *Database) findQueue(name string) *Queue {
	for _, queue := range db.Queues {
		if queue.Name == name {
			return queue
		}
	}
	return nil
}

func (db *Database) findTree(name string) *RBTree {
	for _, tree := range db.Trees {
		if tree.Name == name {
			return tree
		}
	}
	return nil
}

func (db *Database) findChainTable(name string) *ChainHashTable {
	for _, table := range db.ChainTables {
		if table.Name == name {
			return table
		}
	}
	return nil
}

func (db *Database) findOpenTable(name string) *OpenHashTable {
	for _, table := range db.OpenTables {
		if table.Name == name {
			return table
		}
	}
	return nil
}

func (db *Database) PrintAll() {
	fmt.Println("Все структуры:")
	
	for _, arr := range db.Arrays {
		arr.Print()
	}
	
	for _, list := range db.ForwardLists {
		list.Print()
	}
	
	for _, dlist := range db.DoubleLists {
		dlist.Print()
	}
	
	for _, stack := range db.Stacks {
		stack.Print()
	}
	
	for _, queue := range db.Queues {
		queue.Print()
	}
	
	for _, tree := range db.Trees {
		tree.Print()
	}
	
	for _, table := range db.ChainTables {
		table.Print()
	}
	
	for _, table := range db.OpenTables {
		table.Print()
	}
}

// сериализация в текстовый файл (JSON)
func (db *Database) SaveToTextFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	
	encoder := json.NewEncoder(file)
	encoder.SetIndent("", "  ")
	return encoder.Encode(db)
}

// десериализация из текстового файла (JSON)
func (db *Database) LoadFromTextFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	
	decoder := json.NewDecoder(file)
	return decoder.Decode(db)
}

// сериализация в бинарный файл
func (db *Database) SaveToBinaryFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	
	// Сохраняем все структуры
	data := db.SerializeBinary()
	_, err = file.Write(data)
	return err
}

// десериализация из бинарного файла
func (db *Database) LoadFromBinaryFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	
	// Получаем размер файла
	info, err := file.Stat()
	if err != nil {
		return err
	}
	
	// Читаем все данные
	data := make([]byte, info.Size())
	_, err = file.Read(data)
	if err != nil {
		return err
	}
	
	return db.DeserializeBinary(data)
}

// бинарная сериализация всей базы данных
func (db *Database) SerializeBinary() []byte {
	// Для простоты используем JSON сериализацию в бинарном виде
	data, err := json.Marshal(db)
	if err != nil {
		return []byte{}
	}
	return data
}

// бинарная десериализация всей базы данных
func (db *Database) DeserializeBinary(data []byte) error {
	return json.Unmarshal(data, db)
}

func (db *Database) PrintHelp() {
	helpText := `
Доступные команды:

Массив:
  MPUSHTOEND <имя> <значение>
  MPUSHTOINDEX <имя> <индекс> <значение>
  MGET <имя> <индекс>
  MDEL <имя> <индекс>
  MSET <имя> <индекс> <значение>
  MLENGTH <имя>

Односвязный список:
  FPUSHHEAD <имя> <значение>
  FPUSHTAIL <имя> <значение>
  FDELVALUE <имя> <значение>
  FPUSHAFTER <имя> <цель> <значение>
  FPUSHBEFORE <имя> <цель> <значение>
  FFIND <имя> <значение>
  FGET <имя>

Двусвязный список:
  LPUSHHEAD <имя> <значение>
  LPUSHTAIL <имя> <значение>
  LDELVALUE <имя> <значение>
  LFIND <имя> <значение>
  LGET <имя>

Стек:
  SPUSH <имя> <значение>
  SPOP <имя>
  SGET <имя>

Очередь:
  QPUSH <имя> <значение>
  QPOP <имя>
  QGET <имя>

Красно-черное дерево:
  TINSERT <имя> <значение>
  TSEARCH <имя> <значение>

Хэш-таблица (цепочки):
  CHINSERT <имя> <значение>
  CHCONTAINS <имя> <значение>
  CHREMOVE <имя> <значение>
  CHPRINT <имя>

Хэш-таблица (открытая адресация):
  OHINSERT <имя> <значение>
  OHCONTAINS <имя> <значение>
  OHREMOVE <имя> <значение>
  OHPRINT <имя>

Сохранение и загрузка:
  SAVE_TEXT <файл>     - сохранить в текстовый файл (JSON)
  LOAD_TEXT <файл>     - загрузить из текстового файла (JSON)
  SAVE_BINARY <файл>   - сохранить в бинарный файл
  LOAD_BINARY <файл>   - загрузить из бинарного файла
  
Общие:
  PRINT - вывести все структуры
  help - показать эту справку
`
	fmt.Print(helpText)
}
