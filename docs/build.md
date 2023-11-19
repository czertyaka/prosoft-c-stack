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

### Сборка библиотеки

1. В QtCreator выберите ``Open Project`` и укажите путь до репозитория:
    ![Screenshot from 2023-11-19 23-36-51](https://github.com/czertyaka/prosoft-c-stack/assets/69390349/376e998c-39fa-4670-8a4f-d477c9b81d5d)
2. Выберите ``CMakeLists.txt`` и подтвердите действие:
    ![image](https://github.com/czertyaka/prosoft-c-stack/assets/69390349/a6b2e921-025f-4a62-aa01-6b216c780289)
3. Сконфигурируйте проект:
    ![Screenshot from 2023-11-19 23-40-06](https://github.com/czertyaka/prosoft-c-stack/assets/69390349/045ad414-92c8-4441-ba9d-ddb2949ad5be)
4. Для сборки нажмите на символ молотка в нижнем левом углу или нажмите \<Ctrl-B\>.
    ![Screenshot from 2023-11-19 23-41-08](https://github.com/czertyaka/prosoft-c-stack/assets/69390349/d448a90e-b720-46bf-bd84-7e0d7217a1b8)


### Сборка теста (необязательно)

1. Зайдите в ``Projects``:
    ![Screenshot from 2023-11-19 23-43-19](https://github.com/czertyaka/prosoft-c-stack/assets/69390349/97f544cc-cde0-4e7d-9e93-cfe32e07aa58)
2. Выберите поставьте галочку в чекбоксе ``WITH_TEST`` и нажмите ``Run CMake``
    ![Screenshot from 2023-11-19 23-44-15](https://github.com/czertyaka/prosoft-c-stack/assets/69390349/b65ebe66-a8be-4ecc-8f29-3df825c85027)
3. Запустите сборку.
4. Запустите тесты ``Tools``->``Tests``->``Run All Tests``:
    ![image](https://github.com/czertyaka/prosoft-c-stack/assets/69390349/71d2004d-4da1-4360-98b3-2eafb8038737)
    ![image](https://github.com/czertyaka/prosoft-c-stack/assets/69390349/c18b07c9-0b9b-4e00-a0e6-951588776134)



