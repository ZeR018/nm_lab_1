import tkinter as tk


class Interface:
    def __init__(self, master):
        self.master = master  # инициализируем основное окно
        self.photo = tk.PhotoImage(file='logo.png')  # загрузка иконки приложения
        master.iconphoto(False, self.photo)  # установка иконки
        master.title('Задача Коши для ОДУ')  # заголовок
        master.configure(bg='#ececec')  # фон
        master.minsize(1110, 700)  # минимальный размер окна
        self.x0 = tk.StringVar(master, '1')  # x0
        self.u0 = tk.StringVar(master, '2')  # u0
        self.accuracy = tk.StringVar(master, '0.00000001')  # точность выхода на границу
        self.error = tk.StringVar(master, '0.0001')  # контроль лок. поргрешности
        self.max_step = tk.StringVar(master, '10000000')  # максю число шагов
        self.step = tk.StringVar(master, '0.01')  # начальный шаг
        self.rb_var = tk.IntVar(master)  # хранит 0 или 1 (выход на границу x или u)
        self.rb_var.set(0)  # значение по умолчанию
        self.cb_var = tk.BooleanVar(master)  # хранит True или False (включен ли контроль погр-ти)
        self.cb_var.set(1)  # значение по умолчанию
        self.create_widgets()  # создание виджетов

    def create_widgets(self):
        # место для задачи (нужно будет поставить скрин\текст)
        task_l = tk.Label(text='место место место\nдля для для\nзадачи задачи задачи\nура ура ура', bg='#ececec').grid(
            row=0, column=0, columnspan=2, rowspan=4, padx=(10, 0), pady=(10, 0))

        # начальные условия
        cond_l = tk.Label(text='Начальные условия', bg='#ececec').grid(row=4, column=0, columnspan=2)
        cond_xl = tk.Label(text='x0', bg='#ececec').grid(row=5, column=0, padx=(10, 0), sticky='w')
        cond_xe = tk.Entry(highlightbackground='#cbcbcb', textvariable=self.x0).grid(row=5, column=1, padx=(0, 10))
        cond_ul = tk.Label(text='u0', bg='#ececec').grid(row=6, column=0, padx=(10, 0), sticky='w')
        cond_ue = tk.Entry(highlightbackground='#cbcbcb', textvariable=self.u0).grid(row=6, column=1, padx=(0, 10))

        # кнопка Вычислить
        exec_b = tk.Button(text='Вычислить', bg='#ececec', highlightbackground='#ececec', command=self.execute).grid(
            row=8, column=0, columnspan=2, padx=10, sticky='we')

        # параметры програмы
        accuracy_l = tk.Label(text='Точность выхода на границу', bg='#ececec').grid(row=0, column=2, padx=10,
                                                                                    pady=(10, 0), sticky='w')
        rb1 = tk.Radiobutton(text='x', variable=self.rb_var, value=0, bg='#ececec').grid(row=0, column=3, pady=(10, 0),
                                                                                         sticky='e')
        rb2 = tk.Radiobutton(text='u', variable=self.rb_var, value=1, bg='#ececec').grid(row=0, column=4, pady=(10, 0),
                                                                                         sticky='e')
        accuracy_e = tk.Entry(highlightbackground='#cbcbcb', textvariable=self.accuracy).grid(row=2, column=2,
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
                                                                                               sticky='we')
        step_l = tk.Label(text='Начальный шаг', bg='#ececec').grid(row=7, column=2, padx=10, sticky='w')
        step_e = tk.Entry(highlightbackground='#cbcbcb', textvariable=self.step).grid(row=8, column=2, columnspan=3,
                                                                                      padx=(10, 0), sticky='we')

        # справка (реализовать заполнение в функции execute)
        reference_l = tk.Label(text='Справка', bg='#ececec').grid(row=0, column=5, pady=10, sticky='we')
        reference_t = tk.Text(height=14, highlightbackground='#cbcbcb').grid(row=1, column=5, rowspan=9, padx=(10, 0),
                                                                             sticky='we')

        # таблица

        # график

    #  выполняется при нажатии кнопки "Вычислить"
    def execute(self):
        data = [
            self.x0.get(), self.u0.get(), self.accuracy.get(), self.error.get(), self.max_step.get(), self.step.get(),
            self.rb_var.get(), self.cb_var.get()]
        print(data)


root = tk.Tk()
gui = Interface(root)
root.mainloop()
