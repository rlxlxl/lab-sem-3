package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	db := NewDatabase()
	scanner := bufio.NewScanner(os.Stdin)
	
	fmt.Println("Go Database System")
	fmt.Println("Введите 'help' для справки, 'exit' для выхода")
	
	for {
		fmt.Print("> ")
		if !scanner.Scan() {
			break
		}
		
		command := scanner.Text()
		if command == "exit" {
			break
		}
		
		db.ProcessCommand(command)
	}
}

