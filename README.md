# `rnd_dbseq` - Random de Bruijn sequence

Implementation in C++ and python for the generation of random de Bruijn
sequences of any alphabet.

## Binary alphabet

Generate a **binary** random de Bruijn sequence.

### Compile

```bash
cd binary_alphabet
g++ -O2 rnd_dbseq.cpp -o rnd_dbseq
```

### Usage

Generate a random de Bruijn sequence of order 5.

```bash
rnd_dbseq 5
```

#### Python 

```bash
cd binary_alphabet/python
python rnd_dbseq.py 5
```

## General alphabet

Generate a random de Bruijn sequence for **any alphabet**.

### Compile

```bash
cd general_alphabet
g++ -O2 rnd_dbseq.cpp -o rnd_dbseq
```
### Usage

Generate a random de Bruijn sequence of alphabet (sigma) 4 of order 5.
```bash
rnd_dbseq 4 5
```

#### Python 

```bash
cd general_alphabet/python
python rnd_dbseq.py 4 5
```

