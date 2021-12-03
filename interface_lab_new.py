import tkinter as tk
from ctypes import *
import ctypes
import matplotlib
import matplotlib.pyplot as plt

matplotlib.use('TkAgg')
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from tkinter import ttk, END
from tkinter import *
from PIL import Image, ImageTk


class Interface:
    def __init__(self, master):
        self.master = master  # инициализируем основное окно
        self.photo = tk.PhotoImage(file='logo.png')  # загрузка иконки приложения
        master.iconphoto(False, self.photo)  # установка иконки
        master.title('Лабораторная работа №1 Численное решение задачи Коши для ОДУ группа 381903_2')  # заголовок
        master.configure(bg='#ececec')  # фон
        master.minsize(1530, 670)  # минимальный размер окна

        self.x0 = tk.DoubleVar(master, 1)  # x0
        self.u0 = tk.DoubleVar(master, 1)
        self.U = tk.DoubleVar(master, 1) #U'0
        self.a = tk.DoubleVar(master, 0.1)
        self.b = tk.DoubleVar(master, 2)
        self.border = tk.DoubleVar(master, 10.0)  # правая граница
        self.accuracy = tk.DoubleVar(master, 0.0001)  # точность выхода на правую границу
        self.error = tk.DoubleVar(master, 0.00001)  # контроль лок. поргрешности
        self.max_step = tk.DoubleVar(master, 10000000)  # макс. число шагов
        self.step = tk.DoubleVar(master, 0.01)  # начальный шаг
        self.cb_var = tk.BooleanVar(master)  # хранит True или False (включен ли контроль погр-ти)
        self.gr = tk.IntVar(master)
        self.cb_var.set(1)  # значение по умолчанию
        self.create_widgets()  # создание виджетов


    def create_widgets(self):

        # начальные условия
        cond_l = tk.Label(text='Начальные условия', bg='#ececec', font='Helvetica 10 bold').grid(row=0, column=0,
                                                                                                 columnspan=2, padx=10,
                                                                                                 pady=(10, 0))
        task_l = tk.Label(text='Задача', bg='#ececec').grid(row=1, column=0, padx=(10, 0), sticky='we')
        task_types = ['Тестовая', 'Основная1', 'Основная2']
        self.task_c = ttk.Combobox(values=task_types)
        self.task_c.current(1)
        self.task_c.grid(row=1, column=0, columnspan=2, sticky='WE', padx=10)
        cond_l = tk.Label(text='Начальные условия', bg='#ececec').grid(row=2, column=0, columnspan=2, padx=10, pady=(10, 0))
        u_l = tk.Label(text='x0:', bg='#ececec').grid(row=3, column=0, padx=(10, 0),pady=(10, 10), sticky='we')
        u_e = tk.Entry(highlightbackground='#cbcbcb', textvariable=self.x0).grid(row=3, column=1,
                                                                                          padx=(10, 0), sticky='we')
        u_l = tk.Label(text="U0:", bg='#ececec').grid(row=4, column=0, padx=(10, 0), sticky='we')
        u_e = tk.Entry(highlightbackground='#cbcbcb', textvariable=self.u0).grid(row=4, column=1,
                                                                                          padx=(10, 0), sticky='we')
        u_l = tk.Label(text='a:', bg='#ececec').grid(row=5, column=0, padx=(10, 0), sticky='we')
        u_e = tk.Entry(highlightbackground='#cbcbcb', textvariable=self.a).grid(row=5, column=1,
                                                                                          padx=(10, 0), sticky='we')
        u_l = tk.Label(text='b:', bg='#ececec').grid(row=6, column=0, padx=(10, 0), sticky='we')
        u_e = tk.Entry(highlightbackground='#cbcbcb', textvariable=self.b).grid(row=6, column=1,
                                                                                          padx=(10, 0), sticky='we')

        u_l = tk.Label(text="U':", bg='#ececec').grid(row=7, column=0, padx=(10, 0), sticky='we')
        u_e = tk.Entry(highlightbackground='#cbcbcb', textvariable=self.U).grid(row=7, column=1,
                                                                                          padx=(10, 0), sticky='we')
        
        # график
        self.canvas = tk.Canvas()

        # кнопка Вычислить
        exec_b = tk.Button(text='Вычислить', bg='#ececec', highlightbackground='#ececec', command=self.execute, width = 15).grid(
            row=6, column=5, padx=(10,0), pady=(0, 10), sticky='we')

        self.clear_b = tk.Button(text='Очистить график', bg='#ececec', highlightbackground='#ececec', command=self.cleanPlot, width = 15)

        method_b = tk.Button(text='Метод', bg='#ececec', highlightbackground='#ececec', command=self.method, width = 15).grid(
            row=5, column=6,padx=(10,10), pady=(10, 10), sticky='we')

        task_b = tk.Button(text='Задача', bg='#ececec', highlightbackground='#ececec', command=self.task_window, width = 15).grid(
            row=5, column=5, padx=(10,0), pady=(10, 10), sticky='we')

        # параметры програмы
        param_l = tk.Label(text='Параметры метода', bg='#ececec', font='Helvetica 10 bold').grid(row=0, column=2,
                                                                                                    padx=10,
                                                                                                    columnspan=4,
                                                                                                    pady=(10, 0),
                                                                                                    sticky='we')
        border_l = tk.Label(text='Правая граница', bg='#ececec').grid(row=1, column=2, padx=10,
                                                                      pady=(10, 0), sticky='w')
        border_e = tk.Entry(highlightbackground='#cbcbcb', textvariable=self.border).grid(row=2, column=2,
                                                                                          columnspan=3,
                                                                                          padx=(10, 0), sticky='we')
        error_cb = tk.Checkbutton(bg='#ececec', variable=self.cb_var).grid(row=3, column=3, columnspan=2)
        error_l = tk.Label(text='Контроль лок. погрешности', bg='#ececec').grid(row=3, column=2, padx=10, sticky='w')
        error_e = tk.Entry(highlightbackground='#cbcbcb', textvariable=self.error).grid(row=4, column=2, columnspan=3,
                                                                                        padx=(10, 0), sticky='we')
        max_steps_l = tk.Label(text='Максимальное число шагов', bg='#ececec').grid(row=5, column=2, padx=10, sticky='w')
        max_steps_e = tk.Entry(highlightbackground='#cbcbcb', textvariable=self.max_step).grid(row=6, column=2,
                                                                                               columnspan=3,
                                                                                               padx=(10, 0),
                                                                                               sticky='nwe')
        accuracy_l = tk.Label(text='Точность выхода на границу', bg='#ececec').grid(row=1, column=5, columnspan = 2, padx=10,
                                                                                    sticky='w')
        accuracy_e = tk.Entry(highlightbackground='#cbcbcb', textvariable=self.accuracy).grid(row=2, column=5, columnspan = 2,
                                                                                              padx=(10, 0), sticky='we')
        step_l = tk.Label(text='Начальный шаг', bg='#ececec').grid(row=3, column=5, columnspan = 2, padx=10, sticky='w')
        step_e = tk.Entry(highlightbackground='#cbcbcb', textvariable=self.step).grid(row=4, column=5, columnspan=2,
                                                                                      padx=(10, 0), sticky='we')

        cb = tk.Checkbutton(bg='#ececec', variable=self.gr).grid(row=7, column=3, columnspan=2)
        cb_l = tk.Label(text="Показать график U'(U)", bg='#ececec').grid(row=7, column=2, columnspan = 2, padx=10, sticky='w')


        # справка
        reference_l = tk.Label(text='Справка', bg='#ececec', font='Helvetica 10 bold').grid(row=0, column=7, pady=10,
                                                                                            padx=10, sticky='we')
        self.reference_t = tk.Text(height=10, width=70, highlightbackground='#cbcbcb')
        self.reference_t.grid(row=1, column=7, rowspan=6, padx=(10, 10),
                              sticky='we')
        

        # таблица
    def add_columns(self, columns, **kwargs):
        current_columns = list(self.tables['columns'])
        current_columns = {key:self.tables.heading(key) for key in current_columns}

        self.tables['columns'] = list(current_columns.keys()) + list(columns)
        for key in columns:
            self.tables.heading(key, text=key, **kwargs)
            self.tables.column(key, width=7)

        for key in current_columns:
            state = current_columns[key].pop('state')
            self.tables.heading(key, **current_columns[key])
            self.tables.column(key, width=7)

    def table(self, p, _i, d):  
         heads2 = ['i', 'xi', 'Vi', 'V2i', 'Vi-V2i', 'ОЛП', 'hi',  'C1', 'C2']
         self.tables = ttk.Treeview(self.master, show='headings')
         self.tables['columns'] = heads2
         self.tables.grid(row=9, column=5, columnspan=3, rowspan=3, padx=(10, 0), sticky=tk.NSEW)
         for header in heads2:
             self.tables.heading(header, text=header, anchor='center')
             self.tables.column(header, anchor='center')
             self.tables.column(header, width=7)
         _s = 0
         for z in range(int(_i.value / p['k'])):
             if d[p['e'] + z * p['k']] == 0:
                 if z != 0:
                     _s = "<1e-16"
             else:
                 _s = d[p['e'] + z * p['k']]
             self.tables.insert('', tk.END, values=(
             z, round((d[p['xi'] + z * p['k']]), 4), (d[p['Vi'] + z * p['k']]), (d[p['2Vi'] + z * p['k']]), (d[p['Vi-V2i'] + z * p['k']]),  _s,
             d[p['hi'] + z * p['k']], int(d[p['C1'] + z * p['k']]), int(d[p['C2'] + z * p['k']]), d[p['E'] + z * p['k']], (d[p['Ui'] + z * p['k']])))
       
         if (self.task_c.get()=='Тестовая'):
             self.add_columns(('E', 'Ui'))
          
         scroll_bar1 = Scrollbar(self.master, orient=VERTICAL, command=self.tables.yview)
         scroll_bar1.grid(row=9, column=9, rowspan=2, padx=10, sticky=tk.NSEW)
         self.tables.configure(yscroll=scroll_bar1.set)        

        # график
    def plotOnPlane(self, X, Y, U):
        f = plt.figure(num=2, figsize=(7, 5), dpi=80, facecolor='#ececec')
        fig = plt.subplot(1, 1, 1)
        fig.set_title('График')
        if self.gr.get()==1 and self.task_c.get()=='Основная2':
           fig.plot(Y, U, label = 'Численная траектория')
           fig.set_xlabel('U')
           fig.set_ylabel("U'(U)")
        else:
            fig.plot(X, Y, label = 'Численная траектория')
            if self.task_c.get() == 'Тестовая':
                fig.plot(X, U, label = 'Истинная траектория')
            fig.set_xlabel('x')
            fig.set_ylabel('U(x)')
            fig.legend()
        return f

    def cleanPlot(self):
        plt.cla()
        f = plt.figure(num=2, figsize=(7, 5), dpi=80, facecolor='#ececec')
        fig = plt.subplot(1, 1, 1)
        fig.set_title('График')
        #fig.set_xlabel('x')
        #fig.set_ylabel('U(x)')
        self.canvas.draw()

    def create_form_graph(self, figure):
        self.canvas = FigureCanvasTkAgg(figure, self.master)
        self.canvas.get_tk_widget().grid(row=10, column=1, columnspan=4, rowspan=1)
        self.canvas.draw()

    # справка
    def reference(self, p, _i, d):
        self.reference_t.delete(1.0, END)
        mul_counter = 0
        div_counter = 0
        max_step = self.step.get()
        max_step_x = 0
        min_step = self.step.get()
        min_step_x = 0
        max_error = 0
        max_olp = 0
        max_olp_x = 0
        result_x = self.x0.get()
        result_v = 0
        max_error_point = self.x0.get()
        for z in range(1, int(_i.value / p['k'])):
            if d[p['e'] + z * p['k']] > max_olp:
                max_olp = d[p['e'] + z * p['k']]
                max_olp_x = d[p['xi'] + z * p['k']]
            if d[p['E'] + z * p['k']] > max_error:
                max_error = d[p['E'] + z * p['k']]
                max_error_point = d[p['xi'] + z * p['k']]
            div_counter += d[p['C1'] + z * p['k']]
            mul_counter += d[p['C2'] + z * p['k']]
            if d[p['hi'] + z * p['k']] > max_step:
                max_step = d[p['hi'] + z * p['k']]
                max_step_x = d[p['xi'] + z * p['k']]
            if d[p['hi'] + z * p['k']] < min_step:
                min_step = d[p['hi'] + z * p['k']]
                min_step_x = d[p['xi'] + z * p['k']]
            result_x = d[p['xi'] + z * p['k']]
            result_v = d[p['Vi'] + z * p['k']]
        self.reference_t.insert(1.0, 'Метод РК4\n\n')
        self.reference_t.insert(3.0, 'Число шагов: ' + str(int(_i.value / p['k'])-1) + '\n')
        self.reference_t.insert(4.0, 'Число удвоений: ' + str(int(mul_counter)) + '\n')
        self.reference_t.insert(5.0, 'Число делений: ' + str(int(div_counter)) + '\n')
        self.reference_t.insert(6.0, 'Максимальный шаг: ' + str(max_step) + ', в точке x = ' + str(round(max_step_x, 4)) + '\n')
        self.reference_t.insert(7.0, 'Минимальный шаг: ' + str(min_step) + ', в точке x = ' + str(round(min_step_x, 4)) + '\n')
        self.reference_t.insert(8.0, 'Максимальная глобальная погрешность: ' + str(max_error) + ', в точке x = ' + str(round(max_error_point, 4))+ '\n')
        self.reference_t.insert(9.0, 'Максимальная ОЛП: ' + str(max_olp) + ', в точке x = ' + str(
            round(max_olp_x, 4)) + '\n')
        self.reference_t.insert(10.0, 'Итоговые значения: x = ' + str(round(result_x, 4)) + ', V = ' + str(result_v) + '\n')


    # окно задачи
    def task_window(self):
        task = tk.Toplevel()
        task.title('Задача')
        img = ImageTk.PhotoImage(Image.open("./task.jpg"))
        panel = tk.Label(task, image=img)
        panel.pack(side='bottom', fill='both', expand='yes')
        task.mainloop()

    def method(self):
        task = tk.Toplevel()
        task.title('Описание метода')
        img = ImageTk.PhotoImage(Image.open("./method.jpg"))
        panel = tk.Label(task, image=img)
        panel.pack(side='bottom', fill='both', expand='yes')
        task.mainloop()

    #  выполняется при нажатии кнопки "Вычислить"
    def execute(self):
        p=0
        self.clear_b.grid(
            row=6, column=6, padx=10, pady=(0, 10), sticky='we')
        # записываем начальные условия задачи
        init_params = (c_double * 10)()
        init_params[0] = self.x0.get()
        init_params[1] = self.u0.get()
        init_params[2] = self.step.get()
        init_params[3] = self.a.get() # u'0
        init_params[4] = self.b.get() # a
        init_params[5] = self.U.get() # b
        init_params[6] = self.error.get()
        init_params[7] = self.max_step.get()
        init_params[8] = self.border.get()  # точность выхода на границу
        init_params[9] = self.accuracy.get()
        # записываем параметры чм
        # method_params = (c_double*2)()
        # method_params[0] = self.accuracy.get()  # точность выхода на границу
        # method_params[1] = self.error.get()  # контроль погрешности
        # записываем данные с кнопок (выбор границы / контроль лп)
        button_data = (c_int * 2)()
        button_data[0] = 0  # выбор границы 0 - x, 1 - u
        button_data[1] = self.cb_var.get()  # контроль ЛП True/False
        #if self.task_c.get()=='Основная2':
        #  init_params[3]=0

        # подрубаем dll
        dll = cdll.LoadLibrary("lab_1//x64//Release//lab_1.dll")
        # вроде нужно чтобы работало
        dll.work_RK31R.argtypes = [POINTER(POINTER(c_double))]
        dll.work_RK31R.restype = None
        # ---------------------------------------------------------
        dll.del_mem.argtypes = [POINTER(POINTER(c_double))]
        dll.work_RK31R.restype = None

        # для ракрытия массива
        p = {'k': 10, 'xi': 0, 'Vi': 1, '2Vi': 2, 'Vi-V2i': 3, 'e': 4, 'hi': 5, 'Ui': 6, 'E': 7, 'C1': 8, 'C2': 9}

        # главный массив
        d = POINTER(c_double)()

        # количество эл в массиве
        _i = (c_int)()
        if self.task_c.get() == 'Тестовая':
            _i.value = 0
        elif self.task_c.get() == 'Основная1':
            _i.value = 1
        else:
            _i.value = 2


        #_i.value = 1
        # работа
        dll.work_RK31R(byref(d), byref(init_params), byref(button_data), byref(_i))

        # -------------------------------------------------ИНСТРУКЦИЯ---------------------------------------------------------------------------#
        # --------------------------------------------------------------------------------------------------------------------------------------#
        # #из массива берем переменную x      далее берем строку          и умножаем ее на кратность массива
        #           d[p['x']                           +z                          *p['k']]

        # for z in range(int(_i.value/p['k'])):
        # use_it.append(d[p['x']+z*p['k']])
        # use_it.append(d[p['v1']+z*p['k']])

        # print(use_it) # проверка

        # for z in range(int(_i.value/p['k'])):
        #    print("i: ",z,"\tx: ",d[p['x']+z*p['k']],"\tv: ",d[p['v1']+z*p['k']],"\te: ",d[p['e']+z*p['k']],"\th: ",d[p['h']+z*p['k']],"\tu: ",d[p['u']+z*p['k']],"\tE: ",d[p['E']+z*p['k']],"\tC1: ",d[p['c1']+z*p['k']],"\tC2: ",d[p['c2']+z*p['k']],"\n")
        X = []
        for z in range(int(_i.value / p['k'])):
            X.append(d[p['xi'] + z * p['k']])
        Y = []
        for z in range(int(_i.value / p['k'])):
            Y.append(d[p['Vi'] + z * p['k']])
        U = []
        for z in range(int(_i.value / p['k'])):
            U.append(d[p['Ui'] + z * p['k']])

        # график
        self.figure = self.plotOnPlane(X, Y, U)
        self.create_form_graph(self.figure) 
        
        # таблица
        self.table(p, _i, d)
        self.reference(p, _i, d)

        # удаляем память
        dll.del_mem(byref(d))


root = tk.Tk()
gui = Interface(root)
root.mainloop()