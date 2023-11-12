# Algorytmy Kombinatoryczne w Bioinformatyce - Zadanie 2
Tworzenie grafów sprzężonych

Język: C++ 17

## File Format

- \> - początek grafu
- <GRAPH_NAME> - nazwa grafu,
- <VERTEX_NUMBER> - ilość wierzchołków,
- <JOINTS_NUMBER> - ilość łuków,
- <VERTEX_INDEX> - nazwa wierzchołka (index wierzchołka)
- (<VERTEX_INDEX>-<VERTEX_INDEX>), - łuk, krawędź skierowana, "z - do" wierzchołka
- (<VERTEX_INDEX>), - wierzchołek bez łuków

> [!WARNING]  
> <VERTEX_INDEX> - może być reprezentowany jedynie liczbowo

```
><GRAPH_NAME>,<VERTEX_NUMBER>,<JOINTS_NUMBER>
(<VERTEX_INDEX>-<VERTEX_INDEX>),(<VERTEX_INDEX>),
```

**Przykład:**

```
>Graph_1,6,5
#1(a-c),#2(a-b),#3(c-b),#4(c-e),#5(e-a),
```

![](https://github.com/MihaKon/AKwB_Project2/blob/main/photos/graphVisualiztaion.png?raw=true)
