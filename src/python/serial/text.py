import pandas as pd
from timeit import default_timer as timer

def text_analysis():
    try:    
        articles = pd.read_csv("/opt/datasets/articles1.csv",usecols=[1,2,9])
        articles1 = pd.read_csv("/opt/datasets/articles2.csv",usecols=[1,2,9])
        articles2 = pd.read_csv("/opt/datasets/articles3.csv",usecols=[1,2,9])
        articlesFinal = pd.concat([articles, articles1, articles2])
        articlesFinal.head(5)
        articlesFinal.to_csv('aux.csv', sep='\t')
    except Exception as e:
            print("ERROR "+str(e))
text_analysis()
