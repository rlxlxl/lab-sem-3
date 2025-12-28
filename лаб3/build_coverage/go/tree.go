package main

import (
	"encoding/binary"
	"encoding/json"
	"fmt"
)

type Color bool

const (
	RED   Color = true
	BLACK Color = false
)

type TNode struct {
	Key    int     `json:"key"`
	Color  Color   `json:"color"`
	Left   *TNode  `json:"left"`
	Right  *TNode  `json:"right"`
	Parent *TNode  `json:"parent"`
}

type RBTree struct {
	Name string `json:"name"`
	Root *TNode `json:"root"`
	Nil  *TNode `json:"nil"`
}

func NewRBTree() *RBTree {
	nilNode := &TNode{Color: BLACK}
	return &RBTree{
		Root: nilNode,
		Nil:  nilNode,
	}
}

func (t *RBTree) Insert(key int) {
	z := &TNode{
		Key:    key,
		Color:  RED,
		Left:   t.Nil,
		Right:  t.Nil,
		Parent: t.Nil,
	}
	
	y := t.Nil
	x := t.Root
	
	for x != t.Nil {
		y = x
		if z.Key < x.Key {
			x = x.Left
		} else {
			x = x.Right
		}
	}
	
	z.Parent = y
	if y == t.Nil {
		t.Root = z
	} else if z.Key < y.Key {
		y.Left = z
	} else {
		y.Right = z
	}
	
	t.insertFixup(z)
}

func (t *RBTree) insertFixup(z *TNode) {
	for z.Parent.Color == RED {
		if z.Parent == z.Parent.Parent.Left {
			y := z.Parent.Parent.Right
			if y.Color == RED {
				z.Parent.Color = BLACK
				y.Color = BLACK
				z.Parent.Parent.Color = RED
				z = z.Parent.Parent
			} else {
				if z == z.Parent.Right {
					z = z.Parent
					t.leftRotate(z)
				}
				z.Parent.Color = BLACK
				z.Parent.Parent.Color = RED
				t.rightRotate(z.Parent.Parent)
			}
		} else {
			y := z.Parent.Parent.Left
			if y.Color == RED {
				z.Parent.Color = BLACK
				y.Color = BLACK
				z.Parent.Parent.Color = RED
				z = z.Parent.Parent
			} else {
				if z == z.Parent.Left {
					z = z.Parent
					t.rightRotate(z)
				}
				z.Parent.Color = BLACK
				z.Parent.Parent.Color = RED
				t.leftRotate(z.Parent.Parent)
			}
		}
	}
	t.Root.Color = BLACK
}

func (t *RBTree) leftRotate(x *TNode) {
	y := x.Right
	x.Right = y.Left
	
	if y.Left != t.Nil {
		y.Left.Parent = x
	}
	
	y.Parent = x.Parent
	
	if x.Parent == t.Nil {
		t.Root = y
	} else if x == x.Parent.Left {
		x.Parent.Left = y
	} else {
		x.Parent.Right = y
	}
	
	y.Left = x
	x.Parent = y
}

func (t *RBTree) rightRotate(y *TNode) {
	x := y.Left
	y.Left = x.Right
	
	if x.Right != t.Nil {
		x.Right.Parent = y
	}
	
	x.Parent = y.Parent
	
	if y.Parent == t.Nil {
		t.Root = x
	} else if y == y.Parent.Right {
		y.Parent.Right = x
	} else {
		y.Parent.Left = x
	}
	
	x.Right = y
	y.Parent = x
}

func (t *RBTree) Search(key int) bool {
	return t.searchNode(t.Root, key) != t.Nil
}

func (t *RBTree) searchNode(node *TNode, key int) *TNode {
	if node == t.Nil || key == node.Key {
		return node
	}
	
	if key < node.Key {
		return t.searchNode(node.Left, key)
	}
	return t.searchNode(node.Right, key)
}

func (t *RBTree) Print() {
	if t.Root == t.Nil {
		fmt.Printf("Красно-черное дерево '%s' пустое\n", t.Name)
		return
	}
	
	colorStr := "R"
	if t.Root.Color == BLACK {
		colorStr = "B"
	}
	
	fmt.Printf("Красно-черное дерево '%s' (корень: %d(%s)):\n", 
		t.Name, t.Root.Key, colorStr)
	fmt.Println(" (R - красный, B - черный)")
	t.printHorizontal(t.Root, 0)
}

func (t *RBTree) printHorizontal(node *TNode, level int) {
	if node != t.Nil {
		t.printHorizontal(node.Right, level+1)
		
		for i := 0; i < level; i++ {
			fmt.Print("    ")
		}
		
		color := "R"
		if node.Color == BLACK {
			color = "B"
		}
		fmt.Printf("%d(%s)\n", node.Key, color)
		
		t.printHorizontal(node.Left, level+1)
	}
}

func (t *RBTree) GetAllElements() []int {
	var elements []int
	t.inorderTraversal(t.Root, &elements)
	return elements
}

func (t *RBTree) inorderTraversal(node *TNode, result *[]int) {
	if node == t.Nil {
		return
	}
	t.inorderTraversal(node.Left, result)
	*result = append(*result, node.Key)
	t.inorderTraversal(node.Right, result)
}

func (t *RBTree) SetAllElements(elements []int) {
	nilNode := &TNode{Color: BLACK}
	t.Root = nilNode
	t.Nil = nilNode
	
	for _, key := range elements {
		t.Insert(key)
	}
}

// сериализация
func (t *RBTree) MarshalJSON() ([]byte, error) {
	type Alias RBTree
	return json.Marshal(&struct {
		*Alias
		Elements []int `json:"elements"`
	}{
		Alias:    (*Alias)(t),
		Elements: t.GetAllElements(),
	})
}

// десериализация
func (t *RBTree) UnmarshalJSON(data []byte) error {
	type Alias RBTree
	aux := &struct {
		Elements []int `json:"elements"`
		*Alias
	}{
		Alias: (*Alias)(t),
	}
	
	if err := json.Unmarshal(data, aux); err != nil {
		return err
	}
	
	if aux.Elements != nil {
		t.SetAllElements(aux.Elements)
	}
	
	return nil
}

// бинарная сериализация
func (t *RBTree) SerializeBinary() []byte {
	elements := t.GetAllElements()
	buf := make([]byte, 0)
	
	nameBytes := []byte(t.Name)
	nameLen := len(nameBytes)
	buf = binary.BigEndian.AppendUint32(buf, uint32(nameLen))
	buf = append(buf, nameBytes...)
	
	buf = binary.BigEndian.AppendUint32(buf, uint32(len(elements)))
	
	for _, elem := range elements {
		buf = binary.BigEndian.AppendUint32(buf, uint32(elem))
	}
	
	return buf
}

// бинарная десериализация
func (t *RBTree) DeserializeBinary(data []byte) error {
	offset := 0
	
	nameLen := binary.BigEndian.Uint32(data[offset:])
	offset += 4
	t.Name = string(data[offset : offset+int(nameLen)])
	offset += int(nameLen)
	
	elemCount := int(binary.BigEndian.Uint32(data[offset:]))
	offset += 4
	
	var elements []int
	for i := 0; i < elemCount; i++ {
		elem := int(binary.BigEndian.Uint32(data[offset:]))
		offset += 4
		elements = append(elements, elem)
	}
	
	t.SetAllElements(elements)
	return nil
}
