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

# Authors

### Theoretical results and implementation:

* Zsuzsanna Lipták
* Luca Parmigiani

### Experiments:

* [Luca Parmigiani](https://github.com/lucaparmigiani)

# References and citations

[1] Zsuzsanna Lipták, Luca Parmigiani: A BWT-based algorithm for random de Bruijn sequence construction. Accepted for presentation at: 16th Latin American Theoretical Informatics Symposium (LATIN 2024), Puerto Varas, Chile, 18-22 March 2024. ([LATIN 2024](https://latin2024.cmm.uchile.cl/))

Please cite the following paper:

### conference version
```
@inproceedings{LiptakP24,
  author       = {{\relax Zs}uzsanna Lipt{\'{a}}k and Luca Parmigiani},
  title        = {A {BWT}-based algorithm for random de {Bruijn} sequence construction},
  booktitle    = {16th Latin American Theoretical Informatics Symposium (LATIN 2024)},
  year         = {2024},
}
```

### License

This software is distributed under the [BSD3](https://opensource.org/license/bsd-3-clause), see LICENSE for more information.
