import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

np.random.seed(10)

data = np.random.normal(700, 100, 100)

# statystyki
mean = np.mean(data)
std = np.std(data, ddof=1)
median = np.median(data)
q1 = np.percentile(data, 25)
q3 = np.percentile(data, 75)
iqr = q3 - q1

print("Statystyki bez outlierów")
print("Średnia:", mean)
print("Odchylenie std:", std)
print("Mediana:", median)
print("Q1:", q1)
print("Q3:", q3)
print("IQR:", iqr)

# histogram
plt.figure()
plt.hist(data, bins=10)
plt.title("Histogram - 10 przedziałów")
plt.show()

plt.figure()
plt.hist(data, bins=30)
plt.title("Histogram - 30 przedziałów")
plt.show()

# empiryczna dystrybuanta
sorted_data = np.sort(data)
ecdf = np.arange(1, len(sorted_data)+1) / len(sorted_data)

plt.figure()
plt.step(sorted_data, ecdf)
plt.title("Empiryczna dystrybuanta")
plt.xlabel("Cena mieszkania")
plt.ylabel("F(x)")
plt.show()

# wykres pudełkowy
plt.figure()
plt.boxplot(data)
plt.title("Boxplot cen mieszkań")
plt.show()

# outliery
outliers = np.array([1500,1500,1500,1500,1500])
data_out = np.concatenate([data, outliers])

mean2 = np.mean(data_out)
std2 = np.std(data_out, ddof=1)
median2 = np.median(data_out)
q12 = np.percentile(data_out, 25)
q32 = np.percentile(data_out, 75)
iqr2 = q32 - q12

print("\nStatystyki z outlierami")
print("Średnia:", mean2)
print("Odchylenie std:", std2)
print("Mediana:", median2)
print("Q1:", q12)
print("Q3:", q32)
print("IQR:", iqr2)

# boxplot z outlierami
plt.figure()
plt.boxplot(data_out)
plt.title("Boxplot z outlierami")
plt.show()