# ctack
A stack based language which developt with c.


> [!WARNING]
> This project is under development. DON'T USE in real world!

**Table of Contents**:

- [usage](#usage)
- [syntax](#sytax)
- [Example](#example)
- [Data Types](#data-types)
- [Keywords](#keywords)
  - [Stack Manipulation](#stack-manipulation)
  - [Memory Manipulation](#memory-manipulation)
  - [Arithmetics](#arithmetics)
  - [Logical and Comparison](#logical-and-comparison)
  - [Loops and Statements](#loops-and-statements)
  - [IO](#io)
- [structure of project](#structure-of-project)
- [contributing](#contributing)
- [license](#license)

## Usage
```
ctack main.ctk
```

## Syntax

## Example

## Data types
### Data Types

| Type    | Name     | Value                  | Truthly Interpretation         |
|---------|----------|------------------------|--------------------------------|
| Integer | 10       | Positive or negative whole numbers like `10`, `-10` | Greater than 0 is true, 0 or less is false |
| Boolean | true     | `true`, `false`        | `true` is true, `false` is false |
| String  | "hello world!" | Text strings like `"hello world!"` | Non-empty string is true, empty string `""` is false |
| Null    | null     | `null`                 | Always false                  |

## Keywords

### Stack Manipulation

| Keyword | Description                          | Usage             |
|---------|--------------------------------------|-------------------|
| push    | Pushes a value onto the stack. `push` is optional keyword, you can use directly value | `push 5`, `5`          |
| pop     | Removes the top value from the stack | `pop`             |
| peek    | Peeks at the top value without popping it | `peek`         |
| pick    | Get element from stack with index | `pick 2`         |

### Memory Manipulation

| Keyword | Description                              | Usage                |
|---------|------------------------------------------|----------------------|
| var     | Declares a new variable                  | `var x`              |
| store   | Stores the top stack value into a variable | `store x`            |
| load    | Loads the value of a variable onto the stack | `load x`            |
| delete  | Deletes a variable                       | `delete x`           |

### Arithmetics

| Keyword | Description                          | Usage             |
|---------|--------------------------------------|-------------------|
| +       | Adds the top two values on the stack | `+`               |
| -       | Subtracts the top value from the second top value on the stack | `-` |
| *       | Multiplies the top two values on the stack | `*`             |
| /       | Divides the second top value by the top value on the stack | `/` |

### Logical and Comparison Operators

| Keyword | Description                              | Usage               |
|---------|------------------------------------------|---------------------|
| ==      | Checks if two values are equal           | `10 10 ==`          |
| !=      | Checks if two values are not equal       | `10 5 !=`           |
| !       | Logical NOT, inverts the boolean value   | `true !`            |
| &&      | Logical AND, true if both values are true| `true false &&`     |
| ||      | Logical OR, true if at least one value is true | `true false ||`    |
| <       | Checks if the first value is less than the second | `5 10 <`    |
| <=      | Checks if the first value is less than or equal to the second | `10 10 <=` |
| >       | Checks if the first value is greater than the second | `10 5 >`   |
| >=      | Checks if the first value is greater than or equal to the second | `10 10 >=` |

### Loops and Statements

| Keyword | Description                          | Usage             |
|---------|--------------------------------------|-------------------|
| if      | Begins an if statement, executing subsequent commands if the top stack value is true | `if` |
| else    | Begins an else block, executed if the preceding if condition was false | `else` |
| end     | Ends an if or else block             | `end`             |

### IO

| Keyword | Description                          | Usage             |
|---------|--------------------------------------|-------------------|
| print   | Prints the top value on the stack to the console | `print`     |
| dump    | Dumps all stack contents to the console for debugging | `dump`   |

## Contributing

This project accepts the open source and free software mentality in its main terms.
Development and distribution are free within the framework of the rules specified 
in the license section, BUT the course and mentality of the project depends entirely
on my discretion. Please respect this mentality and contributing rules.

See all rules [CONTRIBUTING.md](./CONTRIBUTING.md)


## License
This project is under the [GPLv3 license](./LICENSE).
