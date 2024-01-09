### Попытка сделать моделирование и визуализацию через «Sympy Mechanics» и «PyDy»

На первый взгляд очень перспективно выглядела идея, формальной постановки модели в Jupyter-ноутбуках, используя систему символических вычислений [Sympy Mechanics](https://docs.sympy.org/latest/modules/physics/mechanics/index.html). 
Т.е. вместо формул на бумаге, тут строго формулируются все системы отсчета, связи, ограничения, тензоры, силы… а потом, 
используя [Kane Method](https://docs.sympy.org/latest/modules/physics/mechanics/kane.html), а потом, практически автоматически, эти дифференциальные уровнения можно решить, численно проинтегрировать для получения траекторий, и визуализировать.
Примерно [так это выглядит для тривиальной модели велосипеда](https://pydy.readthedocs.io/en/stable/examples/carvallo-whipple.html).
Ожидалось, что можно будет попробовать добавлять разные силы, например, для «диссипации» и сделать может даже нетривиальное исследование.

Для изучения и тренировки я выполнял упражнения, и переводил документацию и онлайн-книгу «[Learn Multibody Dynamics](https://moorepants.github.io/learn-multibody-dynamics/)» на русский и в формат юпитер-ноутбуков.
К сожалению, задача оказалось сложнее ожидаемой, нехватка времени, знаний, и столкновение с [разными багами](https://github.com/pydy/pydy/issues/493) не дали мне завершить работу.

Наброски ноутбуков здесь https://github.com/arthur-fomin/2023-physics-project/tree/main/pydy-modelling

Возможно в будущем кто-то сможет получить эту симуляцию, а мне удастся продолжить изучение SymPy, где есть модули не только для механики и но и для [оптики](https://docs.sympy.org/latest/modules/physics/optics/index.html)  и даже [квантовой механики](https://docs.sympy.org/latest/modules/physics/quantum/index.html)
