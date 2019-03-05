import pandas as pd
from timeit import default_timer as timer

try:
        start = timer()
        print("Starting articles concatenation...")
        articles = pd.read_csv("/opt/datasets/articles1.csv",usecols=[1,2,9])
        print("article1.csv loaded")
        articles1 = pd.read_csv("/opt/datasets/articles2.csv",usecols=[1,2,9])
        print("article2.csv loaded")
        articles2 = pd.read_csv("/opt/datasets/articles3.csv",usecols=[1,2,9])
        print("article3.csv loaded")
        articlesFinal = pd.concat([articles, articles1, articles2])
        print("Concatenating Files: articles1.csv - articles2.csv - articles3.csv")
        articlesFinal.to_csv('result.csv', sep='\t')
        print("Concatenation successfull")
        print("File created as: result.csv")
        end = timer()
        print("File pre-processing time was: "+(start-end))
except Exception as e:
        print("ERROR "+str(e))
