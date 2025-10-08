#![allow(unsafe_code)]

use std::collections::HashMap;
use std::env;
use std::fs::File;
use std::io::{self, BufRead, Write};
use std::ptr;

// MyArray - manual dynamic array without Vec
struct MyArray {
    data: *mut String,
    len: usize,
    capacity: usize,
}

impl MyArray {
    fn new() -> Self {
        Self {
            data: ptr::null_mut(),
            len: 0,
            capacity: 0,
        }
    }

    fn push(&mut self, val: String) {
        if self.len == self.capacity {
            let new_capacity = if self.capacity == 0 { 4 } else { self.capacity * 2 };
            let layout = std::alloc::Layout::array::<String>(new_capacity).unwrap();
            let new_data = unsafe { std::alloc::alloc(layout) as *mut String };
            if !self.data.is_null() {
                unsafe { ptr::copy_nonoverlapping(self.data, new_data, self.len) };
                let old_layout = std::alloc::Layout::array::<String>(self.capacity).unwrap();
                unsafe { std::alloc::dealloc(self.data as *mut u8, old_layout) };
            }
            self.data = new_data;
            self.capacity = new_capacity;
        }
        unsafe { ptr::write(self.data.add(self.len), val) };
        self.len += 1;
    }

    fn insert(&mut self, index: usize, val: String) {
        if index > self.len {
            panic!("Index out of bounds");
        }
        self.push(String::new());
        unsafe {
            ptr::copy(self.data.add(index), self.data.add(index + 1), self.len - index - 1);
            ptr::write(self.data.add(index), val);
        }
    }

    fn get(&self, index: usize) -> &String {
        if index >= self.len {
            panic!("Index out of bounds");
        }
        unsafe { &*self.data.add(index) }
    }

    fn remove(&mut self, index: usize) {
        if index >= self.len {
            panic!("Index out of bounds");
        }
        unsafe { ptr::drop_in_place(self.data.add(index)) };
        unsafe { ptr::copy(self.data.add(index + 1), self.data.add(index), self.len - index - 1) };
        self.len -= 1;
    }

    fn set(&mut self, index: usize, val: String) {
        if index >= self.len {
            panic!("Index out of bounds");
        }
        unsafe { ptr::write(self.data.add(index), val) };
    }

    fn len(&self) -> usize {
        self.len
    }

    fn to_serialized(&self) -> String {
        (0..self.len).map(|i| self.get(i).clone()).collect::<Vec<_>>().join("|")
    }

    fn from_serialized(serialized: &str) -> Self {
        let mut arr = Self::new();
        for val in serialized.split('|') {
            if !val.is_empty() {
                arr.push(val.to_string());
            }
        }
        arr
    }

    fn read(&self) -> String {
        (0..self.len).map(|i| self.get(i).as_str()).collect::<Vec<_>>().join(" ")
    }
}

impl Drop for MyArray {
    fn drop(&mut self) {
        if !self.data.is_null() {
            for i in 0..self.len {
                unsafe { ptr::drop_in_place(self.data.add(i)) };
            }
            let layout = std::alloc::Layout::array::<String>(self.capacity).unwrap();
            unsafe { std::alloc::dealloc(self.data as *mut u8, layout) };
        }
    }
}

// SinglyNode
struct SinglyNode {
    val: String,
    next: Option<Box<SinglyNode>>,
}

// SinglyLinkedList
struct SinglyLinkedList {
    head: Option<Box<SinglyNode>>,
}

impl SinglyLinkedList {
    fn new() -> Self {
        Self { head: None }
    }

    fn push_head(&mut self, val: String) {
        let new = Box::new(SinglyNode { val, next: self.head.take() });
        self.head = Some(new);
    }

    fn push_tail(&mut self, val: String) {
        let mut current = &mut self.head;
        while let Some(node) = current {
            current = &mut node.next;
        }
        *current = Some(Box::new(SinglyNode { val, next: None }));
    }

    fn push_after(&mut self, after_val: &str, val: String) {
        let mut current = &mut self.head;
        while let Some(node) = current {
            if node.val == after_val {
                let new = Box::new(SinglyNode { val, next: node.next.take() });
                node.next = Some(new);
                return;
            }
            current = &mut node.next;
        }
    }

    fn push_before(&mut self, before_val: &str, val: String) {
        if let Some(ref mut head_node) = self.head {
            if head_node.val == before_val {
                self.push_head(val);
                return;
            }
        }

        let mut current = &mut self.head;
        while let Some(node) = current {
            if let Some(ref mut next_node) = node.next {
                if next_node.val == before_val {
                    let new_node = Box::new(SinglyNode {
                        val,
                        next: node.next.take(),
                    });
                    node.next = Some(new_node);
                    return;
                }
            }
            current = &mut node.next;
        }
    }

    fn delete(&mut self, val: &str) {
        if let Some(ref mut head_node) = self.head {
            if head_node.val == val {
                self.head = head_node.next.take();
                return;
            }
        }

        let mut current = &mut self.head;
        while let Some(node) = current {
            if let Some(ref mut next_node) = node.next {
                if next_node.val == val {
                    node.next = next_node.next.take();
                    return;
                }
            }
            current = &mut node.next;
        }
    }

    fn search(&self, val: &str) -> bool {
        let mut current = &self.head;
        while let Some(node) = current {
            if node.val == val {
                return true;
            }
            current = &node.next;
        }
        false
    }

    fn to_serialized(&self) -> String {
        let mut vals = Vec::new();
        let mut current = &self.head;
        while let Some(node) = current {
            vals.push(node.val.clone());
            current = &node.next;
        }
        vals.join("|")
    }

    fn from_serialized(serialized: &str) -> Self {
        let mut list = Self::new();
        for val in serialized.split('|').rev() {
            if !val.is_empty() {
                list.push_head(val.to_string());
            }
        }
        list
    }

    fn read(&self) -> String {
        let mut vals = Vec::new();
        let mut current = &self.head;
        while let Some(node) = current {
            vals.push(node.val.as_str());
            current = &node.next;
        }
        vals.join(" ")
    }
}

// DoublyNode
struct DoublyNode {
    val: String,
    prev: *mut DoublyNode,
    next: Option<Box<DoublyNode>>,
}

// DoublyLinkedList
struct DoublyLinkedList {
    head: Option<Box<DoublyNode>>,
    tail: *mut DoublyNode,
}

impl DoublyLinkedList {
    fn new() -> Self {
        Self {
            head: None,
            tail: ptr::null_mut(),
        }
    }

    fn push_head(&mut self, val: String) {
        let mut new_node = Box::new(DoublyNode { val, prev: ptr::null_mut(), next: self.head.take() });
        let new_ptr: *mut DoublyNode = &mut *new_node;
        
        if let Some(ref mut old_head) = new_node.next {
            old_head.prev = new_ptr;
        } else {
            self.tail = new_ptr;
        }
        self.head = Some(new_node);
    }

    fn push_tail(&mut self, val: String) {
        if self.head.is_none() {
            self.push_head(val);
            return;
        }
        
        let mut new_node = Box::new(DoublyNode { 
            val: val.clone(), 
            prev: self.tail, 
            next: None 
        });
        let new_ptr: *mut DoublyNode = &mut *new_node;
        
        unsafe {
            (*self.tail).next = Some(new_node);
        }
        self.tail = new_ptr;
    }

    fn push_after(&mut self, after_val: &str, val: String) {
        let mut current = &mut self.head;
        while let Some(node) = current {
            if node.val == after_val {
                let old_next = node.next.take();
                let mut new_node = Box::new(DoublyNode {
                    val,
                    prev: &mut **node as *mut DoublyNode,
                    next: old_next,
                });
                
                let new_ptr: *mut DoublyNode = &mut *new_node;
                
                if let Some(ref mut next_node) = new_node.next {
                    next_node.prev = new_ptr;
                } else {
                    self.tail = new_ptr;
                }
                
                node.next = Some(new_node);
                return;
            }
            current = &mut node.next;
        }
    }

    fn push_before(&mut self, before_val: &str, val: String) {
        // Handle case where we insert before head
        if let Some(ref mut head_node) = self.head {
            if head_node.val == before_val {
                self.push_head(val);
                return;
            }
        }

        let mut current = &mut self.head;
        while let Some(node) = current {
            if let Some(ref mut next_node) = node.next {
                if next_node.val == before_val {
                    let mut new_node = Box::new(DoublyNode {
                        val,
                        prev: &mut **node as *mut DoublyNode,
                        next: node.next.take(),
                    });
                    
                    let new_ptr: *mut DoublyNode = &mut *new_node;
                    
                    if let Some(ref mut new_next) = new_node.next {
                        new_next.prev = new_ptr;
                    }
                    
                    node.next = Some(new_node);
                    return;
                }
            }
            current = &mut node.next;
        }
    }

    fn delete(&mut self, val: &str) {
        // Handle head deletion
        if let Some(ref mut head_node) = self.head {
            if head_node.val == val {
                let mut next = head_node.next.take();
                if let Some(ref mut next_node) = next {
                    next_node.prev = ptr::null_mut();
                } else {
                    self.tail = ptr::null_mut();
                }
                self.head = next;
                return;
            }
        }

        let mut current = &mut self.head;
        while let Some(node) = current {
            if let Some(ref mut next_node) = node.next {
                if next_node.val == val {
                    let mut next_next = next_node.next.take();
                    if let Some(ref mut next_next_node) = next_next {
                        next_next_node.prev = &mut **node as *mut DoublyNode;
                    } else {
                        self.tail = &mut **node as *mut DoublyNode;
                    }
                    node.next = next_next;
                    return;
                }
            }
            current = &mut node.next;
        }
    }

    fn search(&self, val: &str) -> bool {
        let mut current = &self.head;
        while let Some(node) = current {
            if node.val == val {
                return true;
            }
            current = &node.next;
        }
        false
    }

    fn to_serialized(&self) -> String {
        let mut vals = Vec::new();
        let mut current = &self.head;
        while let Some(node) = current {
            vals.push(node.val.clone());
            current = &node.next;
        }
        vals.join("|")
    }

    fn from_serialized(serialized: &str) -> Self {
        let mut list = Self::new();
        for val in serialized.split('|') {
            if !val.is_empty() {
                list.push_tail(val.to_string());
            }
        }
        list
    }

    fn read(&self) -> String {
        let mut vals = Vec::new();
        let mut current = &self.head;
        while let Some(node) = current {
            vals.push(node.val.as_str());
            current = &node.next;
        }
        vals.join(" ")
    }
}

impl Drop for DoublyLinkedList {
    fn drop(&mut self) {
        let mut current = self.head.take();
        while let Some(mut node) = current {
            current = node.next.take();
        }
    }
}

// MyStack
struct MyStack {
    head: Option<Box<SinglyNode>>,
}

impl MyStack {
    fn new() -> Self {
        Self { head: None }
    }

    fn push(&mut self, val: String) {
        let new = Box::new(SinglyNode { val, next: self.head.take() });
        self.head = Some(new);
    }

    fn pop(&mut self) -> Option<String> {
        self.head.take().map(|node| {
            self.head = node.next;
            node.val
        })
    }

    fn get(&self) -> Option<&String> {
        self.head.as_ref().map(|node| &node.val)
    }

    fn to_serialized(&self) -> String {
        let mut vals = Vec::new();
        let mut current = &self.head;
        while let Some(node) = current {
            vals.push(node.val.clone());
            current = &node.next;
        }
        vals.join("|")
    }

    fn from_serialized(serialized: &str) -> Self {
        let mut stack = Self::new();
        for val in serialized.split('|').rev() {
            if !val.is_empty() {
                stack.push(val.to_string());
            }
        }
        stack
    }

    fn read(&self) -> String {
        let mut vals = Vec::new();
        let mut current = &self.head;
        while let Some(node) = current {
            vals.push(node.val.as_str());
            current = &node.next;
        }
        vals.join(" ")
    }
}

// MyQueue
struct MyQueue {
    head: Option<Box<SinglyNode>>,
    tail: *mut SinglyNode,
}

impl MyQueue {
    fn new() -> Self {
        Self { head: None, tail: ptr::null_mut() }
    }

    fn push(&mut self, val: String) {
        let mut new_node = Box::new(SinglyNode { val, next: None });
        let new_ptr: *mut SinglyNode = &mut *new_node;
        
        if !self.tail.is_null() {
            unsafe {
                (*self.tail).next = Some(new_node);
            }
        } else {
            self.head = Some(new_node);
        }
        self.tail = new_ptr;
    }

    fn pop(&mut self) -> Option<String> {
        self.head.take().map(|node| {
            self.head = node.next;
            if self.head.is_none() {
                self.tail = ptr::null_mut();
            }
            node.val
        })
    }

    fn get(&self) -> Option<&String> {
        self.head.as_ref().map(|node| &node.val)
    }

    fn to_serialized(&self) -> String {
        let mut vals = Vec::new();
        let mut current = &self.head;
        while let Some(node) = current {
            vals.push(node.val.clone());
            current = &node.next;
        }
        vals.join("|")
    }

    fn from_serialized(serialized: &str) -> Self {
        let mut queue = Self::new();
        for val in serialized.split('|') {
            if !val.is_empty() {
                queue.push(val.to_string());
            }
        }
        queue
    }

    fn read(&self) -> String {
        let mut vals = Vec::new();
        let mut current = &self.head;
        while let Some(node) = current {
            vals.push(node.val.as_str());
            current = &node.next;
        }
        vals.join(" ")
    }
}

impl Drop for MyQueue {
    fn drop(&mut self) {
        let mut current = self.head.take();
        while let Some(node) = current {
            current = node.next;
        }
    }
}

// TreeNode
struct TreeNode {
    val: String,
    left: Option<Box<TreeNode>>,
    right: Option<Box<TreeNode>>,
}

// FullBinaryTree
struct FullBinaryTree {
    root: Option<Box<TreeNode>>,
}

impl FullBinaryTree {
    fn new() -> Self {
        Self { root: None }
    }

    fn insert(&mut self, val: String) {
        if self.root.is_none() {
            self.root = Some(Box::new(TreeNode { val, left: None, right: None }));
            return;
        }
        // Level order to find first node with <2 children
        let mut queue = std::collections::VecDeque::new();
        queue.push_back(self.root.as_mut().unwrap());
        while let Some(node) = queue.pop_front() {
            if node.left.is_none() {
                node.left = Some(Box::new(TreeNode { val, left: None, right: None }));
                return;
            } else {
                queue.push_back(node.left.as_mut().unwrap());
            }
            if node.right.is_none() {
                node.right = Some(Box::new(TreeNode { val, left: None, right: None }));
                return;
            } else {
                queue.push_back(node.right.as_mut().unwrap());
            }
        }
    }

    fn search(&self, val: &str) -> bool {
        if self.root.is_none() {
            return false;
        }
        let mut queue = std::collections::VecDeque::new();
        queue.push_back(self.root.as_ref().unwrap());
        while let Some(node) = queue.pop_front() {
            if node.val == val {
                return true;
            }
            if let Some(ref left) = node.left {
                queue.push_back(left);
            }
            if let Some(ref right) = node.right {
                queue.push_back(right);
            }
        }
        false
    }

    fn is_full(&self) -> bool {
        if self.root.is_none() {
            return true;
        }
        let mut queue = std::collections::VecDeque::new();
        queue.push_back(self.root.as_ref().unwrap());
        while let Some(node) = queue.pop_front() {
            let has_left = node.left.is_some();
            let has_right = node.right.is_some();
            if has_left != has_right {
                return false;
            }
            if has_left {
                queue.push_back(node.left.as_ref().unwrap());
            }
            if has_right {
                queue.push_back(node.right.as_ref().unwrap());
            }
        }
        true
    }

    fn read(&self) -> String {
        if self.root.is_none() {
            return "empty".to_string();
        }
        let mut res = String::new();
        let mut queue = std::collections::VecDeque::new();
        queue.push_back(self.root.as_ref().unwrap());
        while let Some(node) = queue.pop_front() {
            res.push_str(&node.val);
            res.push(' ');
            if let Some(ref left) = node.left {
                queue.push_back(left);
            }
            if let Some(ref right) = node.right {
                queue.push_back(right);
            }
        }
        res.trim().to_string()
    }

    fn serialize(&self) -> String {
        Self::serialize_helper(&self.root)
    }

    fn serialize_helper(node: &Option<Box<TreeNode>>) -> String {
        if node.is_none() {
            return "# ".to_string();
        }
        let n = node.as_ref().unwrap();
        format!("{} {} {}", n.val, Self::serialize_helper(&n.left), Self::serialize_helper(&n.right))
    }

    fn deserialize(serialized: &str) -> Self {
        let mut iter = serialized.split_whitespace();
        let root = Self::deserialize_helper(&mut iter);
        Self { root }
    }

    fn deserialize_helper<'a>(iter: &mut std::str::SplitWhitespace<'a>) -> Option<Box<TreeNode>> {
        if let Some(token) = iter.next() {
            if token == "#" {
                return None;
            }
            let val = token.to_string();
            let left = Self::deserialize_helper(iter);
            let right = Self::deserialize_helper(iter);
            Some(Box::new(TreeNode { val, left, right }))
        } else {
            None
        }
    }
}

// DataStructure
enum DataStructure {
    M(MyArray),
    F(SinglyLinkedList),
    L(DoublyLinkedList),
    S(MyStack),
    Q(MyQueue),
    T(FullBinaryTree),
}

// DB
struct DB {
    structures: HashMap<String, DataStructure>,
}

impl DB {
    fn new() -> Self {
        Self {
            structures: HashMap::new(),
        }
    }

    fn get_mut_or_create_m(&mut self, name: &str) -> &mut MyArray {
        let entry = self.structures.entry(name.to_string()).or_insert_with(|| DataStructure::M(MyArray::new()));
        if let DataStructure::M(ref mut a) = *entry {
            a
        } else {
            panic!("Wrong type for {}", name);
        }
    }

    fn get_mut_or_create_f(&mut self, name: &str) -> &mut SinglyLinkedList {
        let entry = self.structures.entry(name.to_string()).or_insert_with(|| DataStructure::F(SinglyLinkedList::new()));
        if let DataStructure::F(ref mut f) = *entry {
            f
        } else {
            panic!("Wrong type for {}", name);
        }
    }

    fn get_mut_or_create_l(&mut self, name: &str) -> &mut DoublyLinkedList {
        let entry = self.structures.entry(name.to_string()).or_insert_with(|| DataStructure::L(DoublyLinkedList::new()));
        if let DataStructure::L(ref mut l) = *entry {
            l
        } else {
            panic!("Wrong type for {}", name);
        }
    }

    fn get_mut_or_create_s(&mut self, name: &str) -> &mut MyStack {
        let entry = self.structures.entry(name.to_string()).or_insert_with(|| DataStructure::S(MyStack::new()));
        if let DataStructure::S(ref mut s) = *entry {
            s
        } else {
            panic!("Wrong type for {}", name);
        }
    }

    fn get_mut_or_create_q(&mut self, name: &str) -> &mut MyQueue {
        let entry = self.structures.entry(name.to_string()).or_insert_with(|| DataStructure::Q(MyQueue::new()));
        if let DataStructure::Q(ref mut q) = *entry {
            q
        } else {
            panic!("Wrong type for {}", name);
        }
    }

    fn get_mut_or_create_t(&mut self, name: &str) -> &mut FullBinaryTree {
        let entry = self.structures.entry(name.to_string()).or_insert_with(|| DataStructure::T(FullBinaryTree::new()));
        if let DataStructure::T(ref mut t) = *entry {
            t
        } else {
            panic!("Wrong type for {}", name);
        }
    }

    fn get(&self, name: &str) -> Option<&DataStructure> {
        self.structures.get(name)
    }
}

fn load_db(file_path: &str) -> DB {
    let mut db = DB::new();
    if let Ok(file) = File::open(file_path) {
        let reader = io::BufReader::new(file);
        for line in reader.lines() {
            if let Ok(line) = line {
                let parts: Vec<&str> = line.splitn(3, ' ').collect();
                if parts.len() < 3 {
                    continue;
                }
                let typ = parts[0];
                let name = parts[1].to_string();
                let serialized = parts[2];
                match typ {
                    "M" => db.structures.insert(name, DataStructure::M(MyArray::from_serialized(serialized))),
                    "F" => db.structures.insert(name, DataStructure::F(SinglyLinkedList::from_serialized(serialized))),
                    "L" => db.structures.insert(name, DataStructure::L(DoublyLinkedList::from_serialized(serialized))),
                    "S" => db.structures.insert(name, DataStructure::S(MyStack::from_serialized(serialized))),
                    "Q" => db.structures.insert(name, DataStructure::Q(MyQueue::from_serialized(serialized))),
                    "T" => db.structures.insert(name, DataStructure::T(FullBinaryTree::deserialize(serialized))),
                    _ => None,
                };
            }
        }
    }
    db
}

fn save_db(db: &DB, file_path: &str) {
    let mut file = File::create(file_path).unwrap();
    for (name, structure) in &db.structures {
        let (typ, serialized) = match structure {
            DataStructure::M(a) => ("M", a.to_serialized()),
            DataStructure::F(f) => ("F", f.to_serialized()),
            DataStructure::L(l) => ("L", l.to_serialized()),
            DataStructure::S(s) => ("S", s.to_serialized()),
            DataStructure::Q(q) => ("Q", q.to_serialized()),
            DataStructure::T(t) => ("T", t.serialize()),
        };
        writeln!(file, "{} {} {}", typ, name, serialized).unwrap();
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let mut file_path = String::new();
    let mut query = String::new();
    let mut i = 1;
    while i < args.len() {
        if args[i] == "--file" {
            i += 1;
            file_path = args[i].clone();
        } else if args[i] == "--query" {
            i += 1;
            query = args[i].clone();
        }
        i += 1;
    }

    if file_path.is_empty() || query.is_empty() {
        println!("Usage: ./dbms --file file.data --query 'COMMAND'");
        return;
    }

    let mut db = load_db(&file_path);

    // Improved parsing that handles spaces in values
    let parts: Vec<&str> = query.splitn(3, ' ').collect();

    if parts.is_empty() {
        return;
    }

    let command = parts[0];
    
    match command {
        "MPUSH" | "FPUSHHEAD" | "FPUSHTAIL" | "LPUSHHEAD" | "LPUSHTAIL" | "SPUSH" | "QPUSH" | "TINSERT" if parts.len() >= 2 => {
            // For commands with structure name and value (possibly with spaces)
            if parts.len() == 2 {
                println!("Missing value for {}", command);
                return;
            }
            
            let name = parts[1];
            let value = parts[2];
            
            match command {
                "MPUSH" => {
                    db.get_mut_or_create_m(name).push(value.to_string());
                    println!("{}", value);
                }
                "FPUSHHEAD" => {
                    db.get_mut_or_create_f(name).push_head(value.to_string());
                    println!("{}", value);
                }
                "FPUSHTAIL" => {
                    db.get_mut_or_create_f(name).push_tail(value.to_string());
                    println!("{}", value);
                }
                "LPUSHHEAD" => {
                    db.get_mut_or_create_l(name).push_head(value.to_string());
                    println!("{}", value);
                }
                "LPUSHTAIL" => {
                    db.get_mut_or_create_l(name).push_tail(value.to_string());
                    println!("{}", value);
                }
                "SPUSH" => {
                    db.get_mut_or_create_s(name).push(value.to_string());
                    println!("{}", value);
                }
                "QPUSH" => {
                    db.get_mut_or_create_q(name).push(value.to_string());
                    println!("{}", value);
                }
                "TINSERT" => {
                    db.get_mut_or_create_t(name).insert(value.to_string());
                    println!("{}", value);
                }
                _ => unreachable!(),
            }
            save_db(&db, &file_path);
        }
        "MINSERT" | "MSET" if parts.len() >= 4 => {
            let name = parts[1];
            let index_result = parts[2].parse::<usize>();
            let value = parts[3];
            
            let index = match index_result {
                Ok(idx) => idx,
                Err(_) => {
                    println!("Invalid index");
                    return;
                }
            };
            
            match command {
                "MINSERT" => {
                    db.get_mut_or_create_m(name).insert(index, value.to_string());
                    println!("{}", value);
                }
                "MSET" => {
                    db.get_mut_or_create_m(name).set(index, value.to_string());
                    println!("set");
                }
                _ => unreachable!(),
            }
            save_db(&db, &file_path);
        }
        "FPUSHAFTER" | "FPUSHBEFORE" | "LPUSHAFTER" | "LPUSHBEFORE" if parts.len() >= 4 => {
            let name = parts[1];
            let target = parts[2];
            let value = parts[3];
            
            match command {
                "FPUSHAFTER" => {
                    db.get_mut_or_create_f(name).push_after(target, value.to_string());
                    println!("{}", value);
                }
                "FPUSHBEFORE" => {
                    db.get_mut_or_create_f(name).push_before(target, value.to_string());
                    println!("{}", value);
                }
                "LPUSHAFTER" => {
                    db.get_mut_or_create_l(name).push_after(target, value.to_string());
                    println!("{}", value);
                }
                "LPUSHBEFORE" => {
                    db.get_mut_or_create_l(name).push_before(target, value.to_string());
                    println!("{}", value);
                }
                _ => unreachable!(),
            }
            save_db(&db, &file_path);
        }
        "MDEL" | "MGET" | "FDEL" | "FSEARCH" | "LDEL" | "LSEARCH" | "ISMEMBER" if parts.len() == 3 => {
            let name = parts[1];
            let param = parts[2];
            
            match command {
                "MDEL" => {
                    let index_result = param.parse::<usize>();
                    let index = match index_result {
                        Ok(idx) => idx,
                        Err(_) => {
                            println!("Invalid index");
                            return;
                        }
                    };
                    db.get_mut_or_create_m(name).remove(index);
                    println!("deleted");
                }
                "MGET" => {
                    let index_result = param.parse::<usize>();
                    let index = match index_result {
                        Ok(idx) => idx,
                        Err(_) => {
                            println!("Invalid index");
                            return;
                        }
                    };
                    let val = db.get_mut_or_create_m(name).get(index);
                    println!("{}", val);
                }
                "FDEL" => {
                    db.get_mut_or_create_f(name).delete(param);
                    println!("deleted");
                }
                "FSEARCH" => {
                    let found = db.get_mut_or_create_f(name).search(param);
                    println!("{}", if found { "TRUE" } else { "FALSE" });
                }
                "LDEL" => {
                    db.get_mut_or_create_l(name).delete(param);
                    println!("deleted");
                }
                "LSEARCH" => {
                    let found = db.get_mut_or_create_l(name).search(param);
                    println!("{}", if found { "TRUE" } else { "FALSE" });
                }
                "ISMEMBER" => {
                    let found = db.get_mut_or_create_t(name).search(param);
                    println!("{}", if found { "TRUE" } else { "FALSE" });
                }
                _ => unreachable!(),
            }
            save_db(&db, &file_path);
        }
        "MLEN" | "FGET" | "LGET" | "SPOP" | "SGET" | "QPOP" | "QGET" | "TGET" | "TISFULL" | "PRINT" if parts.len() == 2 => {
            let name = parts[1];
            
            match command {
                "MLEN" => {
                    let len = db.get_mut_or_create_m(name).len();
                    println!("{}", len);
                }
                "FGET" => {
                    let read = db.get_mut_or_create_f(name).read();
                    println!("{}", read);
                }
                "LGET" => {
                    let read = db.get_mut_or_create_l(name).read();
                    println!("{}", read);
                }
                "SPOP" => {
                    if let Some(val) = db.get_mut_or_create_s(name).pop() {
                        println!("{}", val);
                    } else {
                        println!("empty");
                    }
                }
                "SGET" => {
                    if let Some(val) = db.get_mut_or_create_s(name).get() {
                        println!("{}", val);
                    } else {
                        println!("empty");
                    }
                }
                "QPOP" => {
                    if let Some(val) = db.get_mut_or_create_q(name).pop() {
                        println!("{}", val);
                    } else {
                        println!("empty");
                    }
                }
                "QGET" => {
                    if let Some(val) = db.get_mut_or_create_q(name).get() {
                        println!("{}", val);
                    } else {
                        println!("empty");
                    }
                }
                "TGET" => {
                    let read = db.get_mut_or_create_t(name).read();
                    println!("{}", read);
                }
                "TISFULL" => {
                    let is_full = db.get_mut_or_create_t(name).is_full();
                    println!("{}", if is_full { "TRUE" } else { "FALSE" });
                }
                "PRINT" => {
                    if let Some(structure) = db.get(name) {
                        let read = match structure {
                            DataStructure::M(a) => a.read(),
                            DataStructure::F(f) => f.read(),
                            DataStructure::L(l) => l.read(),
                            DataStructure::S(s) => s.read(),
                            DataStructure::Q(q) => q.read(),
                            DataStructure::T(t) => t.read(),
                        };
                        println!("{}", read);
                    } else {
                        println!("not found");
                    }
                }
                _ => unreachable!(),
            }
            if command != "PRINT" {
                save_db(&db, &file_path);
            }
        }
        _ => {
            println!("Unknown command or wrong number of arguments: {}", command);
        }
    }
}