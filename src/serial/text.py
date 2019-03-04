import pandas as pd
from timeit import default_timer as timer

def text_analysis():
    try:
        word = ""
        while(word != "/"):
            word = input("Ingrese la palabra, para salir ingrese \"/\" : " ).lower()
            if word == "/":
                print(articles1.head(2))
                print(articles.head(2))
                
                return 0
            start = timer()
            articles = pd.read_csv("articles1.csv",usecols=[1,2,9])
            articles1 = pd.read_csv("articles2.csv",usecols=[1,2,9])
            articles2 = pd.read_csv("articles3.csv",usecols=[1,2,9])
            articlesFinal = pd.concat([articles, articles1, articles2])
            for word2 in articlesFinal.values.tolist():
                title = str(word2[1]).lower()
                news = str(word2[2]).lower()
                count = title.count(word)+news.count(word)
                if count > 0:
                    temp_count.append([count, word2[0], word2[1]])
            news = sorted(temp_count, key=lambda x: x[0])
            news.reverse()
            end = timer()
            calculationTime = end-start
            for i in news[:10]:
                print(i[0], i[1], i[2])
    except Exception as e:
        print("ERROR "+str(e))
        text_analysis()
text_analysis()
