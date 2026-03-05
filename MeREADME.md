NVS:

<!-- zmienic w klasę
handle przekazywać interfacem -->

<!-- adresy przekzywać &
string & name -> nvs żeby nie miało opsji zmienienia w funkcji -->

<!-- ESP_ERROR -> samemu obsługiwać bo wywali esp -->

<!-- {} -> inicjalizować 0 -> bo inaczej śmietnik dam daje -->

<!-- void* arg -> przekazać instancjęklasy
event_handler_arg -> ten sam typ -->

<!-- null co innego w cpp bo to int -->

<!-- jak zrobić żeby nie dać wykorzystać default konstruktora -> jedno słowo -->

<!-- semafor podnosisię i ma wygasać -->

timer w ledzie do przeniesienia ???

<!-- ledblinkbyChange-> coś do pliku -->

<!-- 1 task do leda - nie robić wszędzie tasków -->

jakieś odniesienie się do obiektu z ./   ???

<!-- przenieść łączenie się żeby wszystko nie było w 1 funkcji -->





IBUTTON.HPP
// ~IButton - sprząta po sobie
// isPresed - zwraca czy guzik jest wciśnięty
// setCallback - przyjmuje 'nr telefonu', gdzie będzie dzwonić jak coś się stanie

// virtual - 'menu' - klasa nie pokazuje jak coś zrobić tylko co może zostać zrobione


BUTTON.HPP
// konkretny guzik który ma opis dokładny rzeczy z interfacu(menu) IButton

//void (*user_callback)(bool) = nullptr;
// zmienna z 'nr telefonu' - na początku pusta
// void (*...)(bool) to typ "wskaźnik na funkcję, która przyjmuje bool"


//TaskHandle_t button_task_handle = NULL;
// To jest specjalny identyfikator 
// od systemu (FreeRTOS), żeby Dzwonek wiedział, którego 
// Asystenta ma obudzić (gdybyśmy mieli 10 guzików).

//static const int DEBOUNCE_TIME_MS = 50;
//czas na uspokojenie przy drganiach guzika(drgania styków)
-------------------------------------------------
. $HOME/esp/wizzdev-esp32-szkolenie/externals/esp-idf/export.sh

idf.py build

idf.py flash -p /dev/ttyUSB0

ls -l /dev/ttyUSB*


//mqtt - instalacja
sudo apt install mosquitto-clients

//SERWER MOSQITO TO PRZYJMOWANIA WIADOMOŚCI w terminalu
mosquitto_sub -h test.mosquitto.org -t "esp/test/json" -v

//sprawdzenie działania poprzzezwysłanie 
mosquitto_pub -h test.mosquitto.org -t "esp/test/json" -m "Mqtt test"

# ESP project

< Program handling Led diod and button, with wi-fi conection, read/write to NVS memory, deep sleep added, and cloud communication by MQTT>

Target board - ESP32
Build process is aimed for Ubuntu/Debian based Linux distributions.

# 1. Development
In this README, the following variables are assumed to be defined: `$PROJECT_DIR`, `$BUILD_DIR`, `BUILD_TEST_DIR`.
E.g.: 
```
PROJECT_DIR=`pwd`  # source root - directory of this README
BUILD_DIR=$(realpath $PROJECT_DIR/../build-<project_name>)
BUILD_TEST_DIR=$(realpath $PROJECT_DIR/../build-<project_name>-test)
```

## 1.1. Build environment preparation
Before first build, some one-time preparation is required

### 1.1.1. Don't forget to update the submodules first
```
git submodule init && git submodule update
git submodule update --init --recursive
```
To update the submodules to the tracked branch (set in `.gitmodules` file):
```
git submodule update --remote  
```
It will update the submodules in local repo. To commit them, call `git add path/to/submodule` and commit.

### 1.1.2. Build the esp compiler
```
cd $PROJECT_DIR
cd build
./install_esp32_compiler.sh 
```

## 1.2. Activate the environment in the terminal
Environment needs to be activated for all commands related to building, flashing and monitoring firmware. 
Needs to be run in every terminal.
```
. $PROJECT_DIR/externals/esp-idf/export.sh  
```

## 1.3. Building
To build the firmware, issue the following commands:
```
mkdir $BUILD_DIR
cd $BUILD_DIR
idf.py -B . -C $PROJECT_DIR/ build  
```

## 1.4. IDE editing
At this point you should be able to open the environment in the QtCreator.
 - Start QtCreator (e.g. run `/opt/Qt/Tools/QtCreator/bin/qtcreator`) from a terminal with **active environment**
 - Open the main CMakeLists.txt from the `$PROJECT_DIR/`
 - Import the automatically generated kit (the one that points to your `$BUILD_DIR`) and disable the desktop kit if active.

## 1.5. Flashing
To flash: 

(if not in `$BUILD_DIR` change `./` to appropriate folder) 
```
idf.py -B ./ -C $PROJECT_DIR/ --baud 921600 flash 
```

One can also start the terminal monitor afterwards:
```
idf.py -B ./ -C $PROJECT_DIR/ --baud 921600 flash monitor
```
Other commands to be combined: `build`, `erase_flash`, e.g.:
```
idf.py -B ./ -C ../pockethernet-esp32/ --baud 921600 erase_flash build flash monitor
```

If the board and serial port converter doesn't support automatic control of RESET and BOOT pins control, one may need to drive them manually, e.g. with buttons.

## 1.6. Testing
To prepare the testing environment, build and run tests, issue the following commands: 
```
# Do NOT activate idf.py environment!
mkdir $BUILD_TEST_DIR
cd $BUILD_TEST_DIR
${PROJECT_DIR}/test/build_tests.sh  
```

Now to rebuild and rerun tests issue:
```
make && ./esp32_test_on_pc
```
Tests can be edited and run in QtCreator

## 1.7. Menuconfig
To configure the ESP32 build - run:
```
# Remember to activate the environment
cd $BUILD_DIR
idf.py -B ./ -C $PROJECT_DIR/ menuconfig
```

# 2. Hardware requirements
...


## 1.8. Cloud communication - MQTT reporting
First install needed package
```
sudo apt install mosquitto-clients
```

Listining can be done in terminal on choose url, 
on free mosqito server service
```
mosquitto_sub -h test.mosquitto.org -t "esp/test/json" -v
```

In other window can publish own message to server
```
mosquitto_pub -h test.mosquitto.org -t "esp/test/json" -m "Mqtt test"
```


