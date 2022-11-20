# prosoft-c-stack
Class project for C-language lessons, Prosoft Developer School 2022

## Описание
Проект предназначен для тривиальной реализации структуры данных [стек](https://neerc.ifmo.ru/wiki/index.php?title=%D0%A1%D1%82%D0%B5%D0%BA) в учебных
целях.

В проект включена статическая библиотека `cstack` (файлы [cstack.h](https://github.com/czertyaka/prosoft-c-stack/blob/master/cstack.h) и
[cstack.c](https://github.com/czertyaka/prosoft-c-stack/blob/master/cstack.c)), написанная на C и реализующая структуру данных, и исполняемый файл
для ее тестирования ([test.cpp](https://github.com/czertyaka/prosoft-c-stack/blob/master/test.cpp)), реализованный на C++. Процесс сборки проекта
описан в `CMakeLists.txt`, в `cmake/gtest.cmake` вынесена часть скрипта `cmake`, отвечающая за включение в наш проект фреймворк для тестирования
[Google Test](https://github.com/google/googletest).

## Зависимости
`gcc >= 6.1` или `clang >= 6.0.0` - компиляторы C/С++

`cmake >= 3.5` - генератор скриптов сборки make/Ninja/etc

`libasan` - библиотека адрес-санитайзера

`libubsan` - библиотека санитайзера неопределенного поведения (UB)

Понадобится также система сборки (make/Ninja/etc) по выбору.

## Сборка
Хорошей практикой является разделение директории с исходными файлами и директорию для сборки проекта. Допустим, git-репозиторий находится в директории
`src`:
```bash
$ tree
.
└── src
    ├── cmake
    │   └── gtest.cmake
    ├── CMakeLists.txt
    ├── cstack.c
    ├── cstack.h
    └── test.cpp

2 directories, 5 files
```
В таком случае проект собирать нужно так:
```bash
mkdir build
cd build
cmake ../src
make all
```
Описанная выше процедура носит исключительно рекомендательный характер.

В процессе сборки будет склонирован репозиторий Google Test, будет собрана библиотека тестирования.

## Тестирование
В директории сборки выполнить:
```bash
ctest
```
