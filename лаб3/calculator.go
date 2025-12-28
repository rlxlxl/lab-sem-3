package main

// Calculator представляет простой калькулятор
type Calculator struct {
	result float64
}

// NewCalculator создает новый экземпляр калькулятора
func NewCalculator() *Calculator {
	return &Calculator{result: 0}
}

// Add добавляет значение к результату
func (c *Calculator) Add(value float64) {
	c.result += value
}

// Subtract вычитает значение из результата
func (c *Calculator) Subtract(value float64) {
	c.result -= value
}

// Multiply умножает результат на значение
func (c *Calculator) Multiply(value float64) {
	c.result *= value
}

// Divide делит результат на значение
func (c *Calculator) Divide(value float64) error {
	if value == 0 {
		return ErrDivisionByZero
	}
	c.result /= value
	return nil
}

// GetResult возвращает текущий результат
func (c *Calculator) GetResult() float64 {
	return c.result
}

// Reset сбрасывает результат в ноль
func (c *Calculator) Reset() {
	c.result = 0
}

// Power возводит результат в степень
func (c *Calculator) Power(exponent float64) {
	if c.result == 0 && exponent < 0 {
		c.result = 0
		return
	}
	
	result := 1.0
	absExp := exponent
	if exponent < 0 {
		absExp = -exponent
	}
	
	for i := 0; i < int(absExp); i++ {
		result *= c.result
	}
	
	if exponent < 0 {
		c.result = 1.0 / result
	} else {
		c.result = result
	}
}

// Abs возвращает абсолютное значение результата
func (c *Calculator) Abs() {
	if c.result < 0 {
		c.result = -c.result
	}
}

// MathUtils содержит утилитные математические функции
type MathUtils struct{}

// Factorial вычисляет факториал числа
func (m *MathUtils) Factorial(n int) (int, error) {
	if n < 0 {
		return 0, ErrNegativeNumber
	}
	if n == 0 || n == 1 {
		return 1, nil
	}
	
	result := 1
	for i := 2; i <= n; i++ {
		result *= i
	}
	return result, nil
}

// IsPrime проверяет, является ли число простым
func (m *MathUtils) IsPrime(n int) bool {
	if n < 2 {
		return false
	}
	if n == 2 {
		return true
	}
	if n%2 == 0 {
		return false
	}
	
	for i := 3; i*i <= n; i += 2 {
		if n%i == 0 {
			return false
		}
	}
	return true
}

// GCD вычисляет наибольший общий делитель
func (m *MathUtils) GCD(a, b int) int {
	for b != 0 {
		a, b = b, a%b
	}
	return a
}

// Errors
var (
	ErrDivisionByZero = &MathError{Message: "division by zero"}
	ErrNegativeNumber = &MathError{Message: "negative number not allowed"}
)

// MathError представляет ошибку математической операции
type MathError struct {
	Message string
}

func (e *MathError) Error() string {
	return e.Message
}

