# Сборка проекта

## Содержание
1. [Командная строка Linux](#командная-строка-linux)
2. [Visual Studio](#visual-studio)
3. [QtCreator](#qtcreator)

## Командная строка Linux

### Сборка библиотеки

1. Перейти в директорию с проектом:
    ```sh
    cd prosoft-c-stack
    ```
2. Выполнить конфигурацию `cmake` в директории `build`, которая создастся автоматически:
    ```sh
    cmake -B build
    ```  
3. Собрать проект без распараллеливания или с распараллеливанием:
    ```sh
    cmake --build build
    cmake --build build -j $(nproc)
    ```

### Сборка теста (необязательно)

1. Установить фреймворк [googletest](https://github.com/google/googletest),
    библиотеку адрес-санитайзера и библиотеку санитайзера UB любым известным способом
    (собрать из исходников, скачать с помощью пакетного менеджера системы).
    Для Debian репозиториев:
    ```sh
    sudo apt install build-essential libgtest-dev libgmock-dev
    ```
    Для RedHat репозиториев:
    ```sh
    sudo dnf install gtest-devel gmock-devel libasab libubsan
    ```
    Для ArchLinux:
    ```sh
    pacman -S gtest gmock
    ```
    Я предпочитаю пользоваться пакетным менеджером [conan](https://conan.io/), для которого в репозитории я
    подготовил файл [conanfile.txt](https://github.com/czertyaka/prosoft-c-stack/blob/master/conanfile.txt).
    Если у вас есть этот инструмент и вы с ним знакомы, установить googletest можно с помощью команды:
    ```sh
    conan install conanfile.txt --build=missing
    ```
3. Сконфигурировать `cmake` с опцией `WITH_TEST`:
    ```sh
    cmake -B build -DWITH_TEST=ON
    cmake -B build/Release -DWITH_TEST=ON # если использовали conan
    ```
4. Собрать проект без распараллеливания или с распараллеливанием:
    ```sh
    cmake --build build
    cmake --build build -j $(nproc)
    ```
5. Запустить выполнение теста:
   ```sh
   ctest --test-dir build/
   ctest --test-dir build/Release # если использовали conan
   ```

## Visual Studio

## QtCreator
