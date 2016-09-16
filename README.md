# Trabajo final de C 2016

## Objetivo

El presente repositorio implementa un generador de índices para archivos CSV.
El trabajo a realizar debe implementar un programa llamado sort que permita
listar los archivos CSV originales en orden ascendente o descendente utilizando estos índices.

## El trabajo a implementar

El trabajo a implementar consiste en utilizar los índices generados por el
indexador (provisto en este mismo repositorio) para listar archivos CSV ordenados
en forma ascendente o descendente, basándose en los archivos de índices generados.
En las secciones siguientes de este documento se especifica el formato de esos
índices.

El binario a desarrollar debe llamarse `sort` y su código principal (programa
`main`) se encuentra implementado con las opciones que recibe el mismo. Usted debe
implementar las funciones de ordenamiento.

La idea es ordenar el índice (sin utilizar el CSV) en memoria según el criterio
especificado y luego recorrer este índice ya ordenado e imprimir en la salida
estándar las líneas del CSV original correspondientes a cada entrada del índice.

### Ejemplo de uso

El uso de `sort` requiere un archivo CSV fuente y un archivo índice creado
previamente.

Asumamos el CSV contiene los siguientes datos:

```csv
30548838;06/07/1992;Aguilera;Fernando
24807867;04/04/1993;Vergara;Ignacio
31343758;15/10/1995;Sanchez;Veronica
32602814;14/02/1992;Barrios;Angela
24021670;23/09/1991;Parra;Xavier
```

> En este ejemplo, el primer campo es el DNI de la persona, el segundo su fecha de
nacimiento y los restantes su apellido y nombre, respectivamente.

Se puede generar el índice por DNI utilizando el siguiente comando:

```bash
./bin/indexer -i input.csv -f 1 -t number -o input-dni.idx
```

El uso del sort de la siguiente forma:

```bash
./bin/sort -d input.csv -i input-dni.idx
```

debería dar como resultado la siguiente salida:

```csv
24021670;23/09/1991;Parra;Xavier
24807867;04/04/1993;Vergara;Ignacio
30548838;06/07/1992;Aguilera;Fernando
31343758;15/10/1995;Sanchez;Veronica
32602814;14/02/1992;Barrios;Angela
```

El uso del sort de la siguiente forma:

```bash
./bin/sort -d input.csv -i input-dni.idx -o desc
```

debería dar como resultado:

```csv
32602814;14/02/1992;Barrios;Angela
31343758;15/10/1995;Sanchez;Veronica
30548838;06/07/1992;Aguilera;Fernando
24807867;04/04/1993;Vergara;Ignacio
24021670;23/09/1991;Parra;Xavier
```

En general, el uso de `sort` sería:

```bash
./bin/sort -d data.csv -i index.idx [-o ASC|DESC]

Donde:
  -o ASC|DESC Se acepta ASC o DESC en minúsculas o mayúsculas. Si no se especifica
              se asume ASC
```

## Descarga y compilación

### Descargar fuentes:

Los fuentes pueden descargarse con el siguiente comando usando GIT

```bash
git clone https://github.com/chrodriguez/c-indexer indexer
```

o puede descargarse directamente en formato **zip** haciendo [click
aquí](https://github.com/chrodriguez/c-indexer/archive/master.zip)

### Compilación

Para compilar los fuentes ingresar al directorio del proyecto y correr:

```bash
make
```

El binario resultante de la compilación quedará en el path `bin/indexer`

#### Generando archivos de prueba

Para generar un conjunto de CSVs de prueba, se provee en el directorio `test/`
un generador de datos aleatorios. Para generar archivos de 10, 100, 1000, 10000,
50000 y 100000 líneas correr el siguiente comando:

```bash
make test
```

Los archivos resultante quedarán en el directorio `test/fixtures`

#### Generando los índices para cada prueba

Una vez creados los datos de prueba, puede crear un índice para cada campo con
el siguiente comando:

```bash
make test-indexes
```

Los archivos de indice resultante quedarán en el directorio `test/output`

## El indexador

El indexador se utiliza de la siguiente forma:

```bash

./bin/indexer -i input.csv -f field_number -t field_type -o index-output.idx


Donde:

  -i  especifica el csv de entrada a indexar

  -f  indica el número de campo a indexar. El primer campo es el numero 1

  -t  indica el tipo del campo a indexar. Los tipos soportados son:
      number: cualquier numero positivo (entero sin signo de 32 bits)
      date: fechas escritas en formato dd/mm/yyyy
      string: cualquier string que no contenga punto y coma

  -o  archivo de indice generado

```

### Ejemplos:

Asumiendo un CSV llamado `input.csv` con el siguiente formato:

```csv
30548838;06/07/1992;Aguilera;Fernando
24807867;04/04/1993;Vergara;Ignacio
31343758;15/10/1995;Sanchez;Veronica
32602814;14/02/1992;Barrios;Angela
24021670;23/09/1991;Parra;Xavier
```

Podremos crear indices para cada uno de los campos del CSV:

```bash
  ./bin/indexer -i input.csv -f 1 -t number -o input-dni.idx
  ./bin/indexer -i input.csv -f 2 -t date -o input-nacimiento.idx
  ./bin/indexer -i input.csv -f 3 -t string -o input-apellido.idx
  ./bin/indexer -i input.csv -f 4 -t string -o input-nombre.idx
```


### Precondiciones

Debido que los archivos CSV no son tan simples como parecen, imponemos las
siguientes precondiciones:

* Las líneas no pueden ser más grandes que **2047 bytes**
* El formato del archivo es CSV, donde los campos se separan por **; (punto y coma)** y los campos
  *no pueden* tener como caracteres ; (punto y coma)
* Todos los campos en el archivo **tienen** la misma cantidad de campos
* El primer campo es dato y **no contiene el encabezado** de los campos

### Formato del indice

El archivo de indice se compone de la siguiente forma:

1. **MAGIC_NUMBER:** son los siguientes bytes: `IDX_CSV`
2. **FIELD_NUMBER:** un número de 16 bits (uint16_t) expresado en **big endian** que indica el numero del campo indexado (El primer campo es el 1)
3. **FIELD_FORMAT:** un numero de 16 bits (uint16_t) expresado en **big endian** que indica el formato del campo indexado (0 number, 1 date, 2 string)
4.**LIST_OF_FILE_POSITIONS:** una lista de valores que describen cada entrada del índice, a decir:
  1. Un número de 64 bits (uint64_t) expresados en **big endian** que indica la posición donde inicia la línea del archivo CSV indexado
  2. Un numero de 16 bits (uint16_t) expresado en **big endian** que indica la longitud del campo indexado
  3. El valor del índice, esto es el string del campo original en el CSV
     indexado. Este campo son los bytes del string, sin el `\0`

#### Ejemplo del contenido de un índice

Asumiendo el CSV siguiente

```csv
30548838;06/07/1992;Aguilera;Fernando
24807867;04/04/1993;Vergara;Ignacio
31343758;15/10/1995;Sanchez;Veronica
32602814;14/02/1992;Barrios;Angela
24021670;23/09/1991;Parra;Xavier
```

El índice quedaría:

```hex
000000 49 44 58 5f 43 53 56 00 02 00 01 00 00 00 00 00  >IDX_CSV.........<
000010 00 00 00 00 0a 30 36 2f 30 37 2f 31 39 39 32 00  >.....06/07/1992.<
000020 00 00 00 00 00 00 26 00 0a 30 34 2f 30 34 2f 31  >......&..04/04/1<
000030 39 39 33 00 00 00 00 00 00 00 4a 00 0a 31 35 2f  >993.......J..15/<
000040 31 30 2f 31 39 39 35 00 00 00 00 00 00 00 6f 00  >10/1995.......o.<
000050 0a 31 34 2f 30 32 2f 31 39 39 32 00 00 00 00 00  >.14/02/1992.....<
000060 00 00 92 00 0a 32 33 2f 30 39 2f 31 39 39 31     >.....23/09/1991<
00006f
```

Como puede verse en el volcado en hexadecimal (16 bytes por línea) del archivo,
el contenido sería como sigue:

* Los primeros 7 bytes corresponden al MAGIC_NUMBER **IDX_CSV** sin `0` *- bytes
  del 0 al 6-*
* A continuación el campo indexado en big endian usando 16 bits. En el volcado
  hexadecimal, puede verse el número **0x0002** *-bytes del 7 al 8-*
* A continuación el campo de formato del campo. En el volcado hexadecimal, puede
  verse el número **0x0001** *-bytes del 9 al 10-
* Luego arranca la lista de valores indexados:
  * Linea 1 del archivo:
      * Posición en el archivo en big endian usando 64 bits. En el volcado de
        memoria **0x0000000000000000** -bytes del 11 al 18-
      * Longitud del texto indexado expresado  en bug endian usando 16 bits. En
        el volcado de memoria **0x000a** - bytes 19 al 20-
      * Texto indexado que ocupa 0x000a bytes (10 bytes) expresado en ASCII y
        sin `\0`. En el volcado de memoria **06/07/1992**
  * Linea 2 a la 5 del archivo continúan con el mismo criteri continúan con el
    mismo criterio

