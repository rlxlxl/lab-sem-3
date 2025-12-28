## Лабораторная работа №6 — PostgreSQL + C++

**Тема БД**: библиотека (книги, авторы, читатели, выдачи, жанры).

### Структура проекта

- `schema.sql` — создание 5 связанных таблиц и вставка тестовых данных.
- `queries.sql` — 10 разных SQL‑запросов (JOIN, WHERE, HAVING, агрегаты, подзапрос, UNION и др.).
- `main.cpp` — простая C++ программа, которая:
  - подключается к PostgreSQL;
  - выполняет `schema.sql` (создаёт таблицы и заполняет данными);
  - выполняет несколько запросов и выводит результаты;
  - показывает пример подготовленного запроса (защита от SQL‑инъекций).

### Подготовка БД PostgreSQL

#### 1. Установка PostgreSQL (если ещё не установлен)

**На macOS через Homebrew:**
```bash
brew install postgresql@14
# или последняя версия:
brew install postgresql
```

**Запуск PostgreSQL:**
```bash
# Запустить службу PostgreSQL (версия 14)
brew services start postgresql@14

# Если у вас другая версия, проверьте установленную:
brew list | grep postgresql

# Или для одноразового запуска:
pg_ctl -D /opt/homebrew/var/postgresql@14 start
```

**Проверка, что PostgreSQL запущен:**
```bash
psql postgres
# Если подключились успешно, введите \q для выхода
```

#### 2. Создание базы данных

Подключитесь к PostgreSQL и создайте базу данных:

```bash
psql postgres
```

В консоли PostgreSQL выполните:
```sql
CREATE DATABASE lab6;
\q
```

#### 3. Настройка пользователя и пароля

Если нужно создать пользователя `postgres` с паролем `postgres`:

```bash
psql postgres
```

```sql
-- Создать пользователя (если не существует)
CREATE USER postgres WITH PASSWORD 'postgres';
-- Дать права на создание БД
ALTER USER postgres CREATEDB;
-- Дать права на базу lab6
GRANT ALL PRIVILEGES ON DATABASE lab6 TO postgres;
\q
```

**Или используйте существующего пользователя** — измените строку подключения в `main.cpp`:

```cpp
std::string connection_str =
    "dbname=lab6 user=ваш_пользователь password=ваш_пароль host=localhost port=5432";
```

**Примечание:** Если PostgreSQL установлен через Homebrew, по умолчанию пользователь — это ваш системный пользователь (без пароля). В этом случае можно использовать:
```cpp
std::string connection_str = "dbname=lab6 host=localhost port=5432";
```

### Сборка (через g++)

#### 1. Установка библиотеки libpqxx

**На macOS через Homebrew:**
```bash
brew install libpqxx
```

#### 2. Компиляция

**Стандартная команда (если компилятор находит библиотеки автоматически):**
```bash
g++ -std=c++17 main.cpp -o lab6_app -lpqxx -lpq
```

**Если возникает ошибка "file not found" или "library 'pq' not found", укажите пути явно:**
```bash
g++ -std=c++17 main.cpp -o lab6_app \
  -I/opt/homebrew/include \
  -L/opt/homebrew/lib \
  -L/opt/homebrew/lib/postgresql@14 \
  -lpqxx -lpq
```

**Или используйте pkg-config для автоматического определения путей:**
```bash
g++ -std=c++17 main.cpp -o lab6_app \
  -I/opt/homebrew/include \
  -L/opt/homebrew/lib \
  $(pkg-config --libs libpq) \
  -lpqxx
```

**Важно:** Библиотека `libpqxx` требует стандарт C++17 или выше, поэтому обязательно используйте флаг `-std=c++17`.

**Или используйте pkg-config (если установлен):**
```bash
g++ -std=c++17 main.cpp -o lab6_app \
  $(pkg-config --cflags --libs libpqxx) \
  -lpq
```

### Запуск

```bash
./lab6_app
```

Программа:

- создаст таблицы и данные (через `schema.sql`);
- выполнит несколько запросов;
- выведет результаты в консоль.

### Решение типичных проблем

#### Ошибка подключения к БД

**Проблема:** `could not connect to server` или `password authentication failed`

**Решение:**
1. Убедитесь, что PostgreSQL запущен:
   ```bash
   brew services list | grep postgresql
   # Если не запущен, запустите (замените @14 на вашу версию):
   brew services start postgresql@14
   # Или проверьте установленную версию:
   brew list | grep postgresql
   ```

2. Проверьте параметры подключения в `main.cpp` (строка 25-26):
   - Убедитесь, что база данных `lab6` существует
   - Проверьте имя пользователя и пароль
   - Если используете системного пользователя macOS, можно убрать пароль:
     ```cpp
     std::string connection_str = "dbname=lab6 host=localhost port=5432";
     ```

#### Ошибка компиляции: `pqxx/pqxx: file not found` или `libpqxx/pqxx.h: No such file or directory`

**Решение:**

1. **Установите libpqxx:**
   ```bash
   brew install libpqxx
   ```

2. **После установки используйте команду с явными путями и стандартом C++17:**
   ```bash
   g++ -std=c++17 main.cpp -o lab6_app \
     -I/opt/homebrew/include \
     -L/opt/homebrew/lib \
     -L/opt/homebrew/opt/postgresql@14/lib \
     -lpqxx -lpq
   ```
   
   **Если у вас другая версия PostgreSQL, замените `@14` на вашу версию.**
   
   **Важно:** Обязательно добавьте флаг `-std=c++17`, так как `libpqxx` требует C++17 или выше.

3. **Проверьте, что библиотека установлена:**
   ```bash
   ls /opt/homebrew/include/pqxx
   ls /opt/homebrew/lib/libpqxx*
   ```

4. **Если файлы находятся в другом месте, найдите их:**
   ```bash
   find /opt/homebrew -name "pqxx.h" 2>/dev/null
   find /opt/homebrew -name "libpqxx*" 2>/dev/null
   ```
   И используйте найденные пути в команде компиляции.

#### Ошибка компиляции: `nested namespace definition is a C++17 extension` или `no template named 'is_signed_v'`

**Проблема:** Компилятор использует старый стандарт C++ (C++11/C++14), а `libpqxx` требует C++17.

**Решение:**
Добавьте флаг `-std=c++17` в команду компиляции:

```bash
g++ -std=c++17 main.cpp -o lab6_app \
  -I/opt/homebrew/include \
  -L/opt/homebrew/lib \
  -lpqxx -lpq
```

**Или для более новой версии:**
```bash
g++ -std=c++20 main.cpp -o lab6_app \
  -I/opt/homebrew/include \
  -L/opt/homebrew/lib \
  -lpqxx -lpq
```

#### Ошибка линковки: `library 'pq' not found` или `ld: library not found for -lpq`

**Проблема:** Линковщик не может найти библиотеку `libpq` (клиентская библиотека PostgreSQL).

**Решение:**

1. **Добавьте путь к библиотеке PostgreSQL:**
   ```bash
   g++ -std=c++17 main.cpp -o lab6_app \
     -I/opt/homebrew/include \
     -L/opt/homebrew/lib \
     -L/opt/homebrew/lib/postgresql@14 \
     -lpqxx -lpq
   ```

2. **Если у вас другая версия PostgreSQL, найдите правильный путь:**
   ```bash
   find /opt/homebrew -name "libpq.dylib" 2>/dev/null
   ```
   И используйте найденный путь (директорию, где находится файл): `-L/путь/к/директории`

3. **Или используйте pkg-config:**
   ```bash
   g++ -std=c++17 main.cpp -o lab6_app \
     -I/opt/homebrew/include \
     -L/opt/homebrew/lib \
     $(pkg-config --libs libpq) \
     -lpqxx
   ```

#### Ошибка: `file not found: schema.sql`

**Решение:**
Убедитесь, что вы запускаете программу из директории проекта:
```bash
cd "/Users/a1111/Desktop/STUD/SECOND COURSE/third semester/programming/lab/lab6"
./lab6_app
```

#### Ошибка: `relation "books" already exists`

**Решение:**
Это нормально — программа пытается создать таблицы, которые уже существуют. Файл `schema.sql` использует `DROP TABLE IF EXISTS`, поэтому таблицы будут пересозданы. Если ошибка повторяется, удалите базу и создайте заново:
```sql
DROP DATABASE lab6;
CREATE DATABASE lab6;
```


