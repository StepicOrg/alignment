from collections import defaultdict
import stepik_alignment


def form_matrix(substitution_matrix):
    matrix = defaultdict(dict)
    for (a, b), v in substitution_matrix.items():
        matrix[a][b] = v
        matrix[b][a] = v
    return matrix


seq1 = [c for c in 'AAABB']
seq2 = [c for c in 'BBч']
gap_open = 1
gap_extend = 1
substitution_matrix = {('A', 'A'): 1, ('B', 'B'): 2, ('A', 'B'): 0}
substitution_matrix = {('A', 'A'): 1, ('ч', 'ч'): 1, ('B', 'B'): 2, ('A', 'B'): 0}
matrix = form_matrix(substitution_matrix)
print(matrix)

alignment = stepik_alignment.alignment(seq1, seq2, gap_open, gap_extend, matrix, False)
print(alignment.score)
print(list(alignment.s1))
print(list(alignment.s2))
