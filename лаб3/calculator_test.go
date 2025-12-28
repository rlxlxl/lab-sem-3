package main

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
	"github.com/stretchr/testify/suite"
)

// CalculatorTestSuite представляет набор тестов для Calculator
type CalculatorTestSuite struct {
	suite.Suite
	calc *Calculator
}

// SetupTest вызывается перед каждым тестом
func (suite *CalculatorTestSuite) SetupTest() {
	suite.calc = NewCalculator()
}

// TestCalculatorSuite запускает все тесты из CalculatorTestSuite
func TestCalculatorSuite(t *testing.T) {
	suite.Run(t, new(CalculatorTestSuite))
}

// TestAdd проверяет функцию Add
func (suite *CalculatorTestSuite) TestAdd() {
	suite.calc.Add(5)
	assert.Equal(suite.T(), 5.0, suite.calc.GetResult())
	
	suite.calc.Add(3.5)
	assert.Equal(suite.T(), 8.5, suite.calc.GetResult())
}

// TestSubtract проверяет функцию Subtract
func (suite *CalculatorTestSuite) TestSubtract() {
	suite.calc.Add(10)
	suite.calc.Subtract(3)
	assert.Equal(suite.T(), 7.0, suite.calc.GetResult())
	
	suite.calc.Subtract(2.5)
	assert.Equal(suite.T(), 4.5, suite.calc.GetResult())
}

// TestMultiply проверяет функцию Multiply
func (suite *CalculatorTestSuite) TestMultiply() {
	suite.calc.Add(5)
	suite.calc.Multiply(3)
	assert.Equal(suite.T(), 15.0, suite.calc.GetResult())
	
	suite.calc.Multiply(0.5)
	assert.Equal(suite.T(), 7.5, suite.calc.GetResult())
}

// TestDivide проверяет функцию Divide
func (suite *CalculatorTestSuite) TestDivide() {
	suite.calc.Add(10)
	err := suite.calc.Divide(2)
	require.NoError(suite.T(), err)
	assert.Equal(suite.T(), 5.0, suite.calc.GetResult())
	
	err = suite.calc.Divide(0)
	assert.Error(suite.T(), err)
	assert.Equal(suite.T(), ErrDivisionByZero, err)
}

// TestGetResult проверяет функцию GetResult
func (suite *CalculatorTestSuite) TestGetResult() {
	assert.Equal(suite.T(), 0.0, suite.calc.GetResult())
	
	suite.calc.Add(42)
	assert.Equal(suite.T(), 42.0, suite.calc.GetResult())
}

// TestReset проверяет функцию Reset
func (suite *CalculatorTestSuite) TestReset() {
	suite.calc.Add(100)
	suite.calc.Reset()
	assert.Equal(suite.T(), 0.0, suite.calc.GetResult())
}

// TestPower проверяет функцию Power
func (suite *CalculatorTestSuite) TestPower() {
	suite.calc.Add(2)
	suite.calc.Power(3)
	assert.Equal(suite.T(), 8.0, suite.calc.GetResult())
	
	suite.calc.Reset()
	suite.calc.Add(3)
	suite.calc.Power(2)
	assert.Equal(suite.T(), 9.0, suite.calc.GetResult())
}

// TestAbs проверяет функцию Abs
func (suite *CalculatorTestSuite) TestAbs() {
	suite.calc.Add(-10)
	suite.calc.Abs()
	assert.Equal(suite.T(), 10.0, suite.calc.GetResult())
	
	suite.calc.Reset()
	suite.calc.Add(5)
	suite.calc.Abs()
	assert.Equal(suite.T(), 5.0, suite.calc.GetResult())
}

// Отдельные тесты без suite

// TestNewCalculator проверяет создание нового калькулятора
func TestNewCalculator(t *testing.T) {
	calc := NewCalculator()
	assert.NotNil(t, calc)
	assert.Equal(t, 0.0, calc.GetResult())
}

// TestCalculatorComplexOperations проверяет сложные операции
func TestCalculatorComplexOperations(t *testing.T) {
	calc := NewCalculator()
	
	calc.Add(10)
	calc.Multiply(2)
	calc.Subtract(5)
	calc.Divide(3)
	
	assert.InDelta(t, 5.0, calc.GetResult(), 0.001)
}

// MathUtilsTestSuite представляет набор тестов для MathUtils
type MathUtilsTestSuite struct {
	suite.Suite
	utils *MathUtils
}

// SetupTest вызывается перед каждым тестом
func (suite *MathUtilsTestSuite) SetupTest() {
	suite.utils = &MathUtils{}
}

// TestMathUtilsSuite запускает все тесты из MathUtilsTestSuite
func TestMathUtilsSuite(t *testing.T) {
	suite.Run(t, new(MathUtilsTestSuite))
}

// TestFactorial проверяет функцию Factorial
func (suite *MathUtilsTestSuite) TestFactorial() {
	result, err := suite.utils.Factorial(0)
	require.NoError(suite.T(), err)
	assert.Equal(suite.T(), 1, result)
	
	result, err = suite.utils.Factorial(1)
	require.NoError(suite.T(), err)
	assert.Equal(suite.T(), 1, result)
	
	result, err = suite.utils.Factorial(5)
	require.NoError(suite.T(), err)
	assert.Equal(suite.T(), 120, result)
	
	result, err = suite.utils.Factorial(7)
	require.NoError(suite.T(), err)
	assert.Equal(suite.T(), 5040, result)
	
	_, err = suite.utils.Factorial(-1)
	assert.Error(suite.T(), err)
	assert.Equal(suite.T(), ErrNegativeNumber, err)
}

// TestIsPrime проверяет функцию IsPrime
func (suite *MathUtilsTestSuite) TestIsPrime() {
	assert.False(suite.T(), suite.utils.IsPrime(0))
	assert.False(suite.T(), suite.utils.IsPrime(1))
	assert.True(suite.T(), suite.utils.IsPrime(2))
	assert.True(suite.T(), suite.utils.IsPrime(3))
	assert.False(suite.T(), suite.utils.IsPrime(4))
	assert.True(suite.T(), suite.utils.IsPrime(5))
	assert.False(suite.T(), suite.utils.IsPrime(6))
	assert.True(suite.T(), suite.utils.IsPrime(7))
	assert.False(suite.T(), suite.utils.IsPrime(8))
	assert.False(suite.T(), suite.utils.IsPrime(9))
	assert.False(suite.T(), suite.utils.IsPrime(10))
	assert.True(suite.T(), suite.utils.IsPrime(11))
	assert.True(suite.T(), suite.utils.IsPrime(13))
	assert.True(suite.T(), suite.utils.IsPrime(17))
	assert.True(suite.T(), suite.utils.IsPrime(19))
	assert.False(suite.T(), suite.utils.IsPrime(20))
}

// TestGCD проверяет функцию GCD
func (suite *MathUtilsTestSuite) TestGCD() {
	assert.Equal(suite.T(), 6, suite.utils.GCD(12, 18))
	assert.Equal(suite.T(), 1, suite.utils.GCD(17, 19))
	assert.Equal(suite.T(), 5, suite.utils.GCD(25, 15))
	assert.Equal(suite.T(), 1, suite.utils.GCD(1, 1))
	assert.Equal(suite.T(), 7, suite.utils.GCD(14, 21))
}

// TestErrorTypes проверяет типы ошибок
func TestErrorTypes(t *testing.T) {
	assert.Equal(t, "division by zero", ErrDivisionByZero.Error())
	assert.Equal(t, "negative number not allowed", ErrNegativeNumber.Error())
}

