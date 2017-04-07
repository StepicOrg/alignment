from collections import defaultdict
import stepik_alignment

def form_matrix(substitution_matrix):
    matrix = defaultdict(dict)
    for (a, b), v in substitution_matrix.items():
        matrix[a][b] = v
        matrix[b][a] = v
    return matrix

seq1 = 'AAABBB'
seq2 = 'BBB'
gap_open = 1
gap_extend = 1
substitution_matrix = {('A', 'A'): 1, ('B', 'B'): 2, ('A', 'B'): 0}
#TODO: fix for substitution_matrix = {('A', 'A'): 1, ('ч', 'ч'): 1, ('B', 'B'): 2, ('A', 'B'): 0}
matrix = form_matrix(substitution_matrix)
print(matrix)

alignment = stepik_alignment.alignment(seq1, seq2, gap_open, gap_extend, matrix, False)
print(alignment.score)
print(alignment.s1)
print(alignment.s2)