import math

N = 350
min_squares = [0] * N
min_squares[0] = 0
squares_by_count = { 1:[], 2:[], 3:[], 4:[] }

for n in range(1, N):
	i = sq = 1
	minimum = math.inf

	while sq <= n:
		candidate = 1 + min_squares[n - sq]
		if candidate < minimum:
			minimum = candidate

		i += 1
		sq = i*i

	min_squares[n] = minimum
	squares_by_count[minimum].append(n)
	print(n, minimum)

for i in range(1, 5):
	print(squares_by_count[i])