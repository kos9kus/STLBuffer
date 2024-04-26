#  Task

В каждой задаче данного подраздела требуется реализовать менеджер объектов, который будет вести учет всех созданных объектов и иметь возможность в любой момент исполнения программы распечатать все аллоцированные объекты в алфавитном порядке (функция print_objects).

Объекты создаются с помощью интерфейса create_object и удаляются с помощью интерфейса destroy_object. 

Задача: реализовать коллектор живых объектов следующим образом. Каждый объект имеет имя и набор ссылок на другие объекты. Некоторые объекты называются корневыми и считаются живыми. Живым также считается объект доступный по ссылке из корневого объекта или из другого живого объекта.

Цель задачи – после работы программы (создания и удаления объектов, расстановки ссылок) запустить коллектор живых объектов (функция collect_live_objects) и распечатать имена объектов в алфавитном порядке. Расстановка ссылок производится с помощью интерфейса link. Корневыми объекты помечаются с помощью интерфейса set_root.

Реализуемые функции:

create

destroy

create_object 

destroy_object 

print_objects

link 

set_root

collect_live_objects


Для решения задач необходимо использовать структуру memory_manager_t из файла os_mem.h. Там же можно найти более подробное описание каждой функции. Необходимо реализовать все функции, указанные в списке к Вашей задаче, а также функцию setup_memory_manager, которая устанавливает все реализованные функции. Все функции должны быть определены в одном файле, который называется SS_2_NNtask.cpp, где SS - номер студента в списке, NN - номер задачи. Функция main, демонстрирующая работу программы, должна быть определена в файле SS_2_NNmain.cpp. 

Объяснение задач раздела “Менеджер памяти”.

Использовать функции и контейнеры STL не разрешается.

Важно: Все реализуемые функции не должны выводить ничего, кроме того, что требуется этих функциях. Исключение составляет тестирующая функция main - там вывод на экран допускается и даже приветствуется. Также ни функции менеджера памяти, ни функция main не должны запрашивать ввода с клавиатуры, в частности, не должно быть вызова функции getch(), так как она запрашивает ввод с клавиатуры.

Важно: При тестировании на сервере используется специальная утилита проверки корректности работы с памятью. Поэтому если Ваша программа работает с памятью некорректно (например, выходит за границы выделенного с помощью malloc или new адресного пространства), эта утилита выдаст сообщение о такой ошибке и программа завершится со статусом RE (Runtime Error). То, что в Visual Studio программа не вылетает на тех же тестовых данных, аргументом не является, ошибки работы с памятью должны быть устранены.
