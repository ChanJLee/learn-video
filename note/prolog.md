# Prolog syntax and semantics

[from wiki](https://en.wikipedia.org/wiki/Prolog_syntax_and_semantics)

> The syntax and semantics of the Prolog programming language are the set of rules that defines how a Prolog program is written and how it is interpreted. The rules are laid out in ISO standard ISO/IEC 13211[1] although there are differences in the Prolog implementations. 

## DATA TYPES

prolog是动态类型语言，他有单独的data type, term，并且这些data type, term 有一系列的子类型：atoms, numbers, variables and compound terms。

### ATOM

atom就是普通的命名，他没有什么特殊的含义，它有解释器生成，来作为一个处理单元。它通常直接在prolog代码里直接声明，而不需要什么特殊的语法去标识它。但是，如果你的atom中含有空白符号，或者特殊字符，那就必须用单引号包裹。如果一个atom是以大写字母开头的，那也必须用单引号进行包裹。空的链表[]它也是atom。其他的例子有：x, blue, 'Taco', and 'some atom'

### NUMBERS

Numbers can be floats or integers. 有些prolog实现还包括无穷数和有理数。

### VARIABLES

Variables

Rules and facts