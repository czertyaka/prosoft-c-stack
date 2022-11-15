# prosoft-c-stack
Class project for C-language lessons, Prosoft Developer School 2022

## Описание
Проект предназначен для тривиальной реализации структуры данных [стек](https://neerc.ifmo.ru/wiki/index.php?title=%D0%A1%D1%82%D0%B5%D0%BA) в учебных
целях.

В проект включена статическая библиотека `cstack` (файлы `cstack.h` и `cstack.c`), написанная на C и реализующая структуру данных, и исполняемый файл
для ее тестирования (`cstack_test.cpp`), реализованный на C++. Процесс сборки проекта описан в `CMakeLists.txt`, в `cmake/gtest.cmake` вынесена часть
скрипта `cmake`, отвечающая за включение в наш проект [Google Test](https://github.com/google/googletest).

## Зависимости
`gcc`
`g++`
`cmake >= 3.5`

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
