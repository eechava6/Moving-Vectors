import pandas as pd
from timeit import default_timer as timer
import os

try:
        if not(os.path.isfile('./results1.csv') and os.path.isfile('./results2.csv') and os.path.isfile('./results3.csv') ):
                start = timer()
                print("Starting articles concatenation...")
                articles = pd.read_csv("/opt/datasets/articles1.csv",usecols=[1,2,9])
                articles['title']=articles['title'].str.lower()
                articles['content']=articles['content'].str.lower()
                articles.to_csv('results1.csv',index=False,header=False, sep=';')
                print("article1.csv loaded")
                articles1 = pd.read_csv("/opt/datasets/articles2.csv",usecols=[1,2,9])
                articles1['title']=articles1['title'].str.lower()
                articles1['content']=articles1['content'].str.lower()
                articles1.to_csv('results2.csv',index=False,header=False, sep=';')
                print("article2.csv loaded")
                articles2 = pd.read_csv("/opt/datasets/articles3.csv",usecols=[1,2,9])
                articles2['title']=articles2['title'].str.lower()
                articles2['content']=articles2['content'].str.lower()
                articles2.to_csv('results3.csv',index=False,header=False, sep=';')
                print("article3.csv loaded")
                end = timer()
                print("File pre-processing time was: "+str(end-start))
        else:
                print("File 'results.csv' already exists, skypping concatenation ...")
except Exception as e:
        print("ERROR "+str(e))
