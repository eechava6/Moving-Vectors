import pandas as pd

def text_analysis():
    try:
        word = ""
        while(word != "/"):
            word = input("Ingrese la palabra, para salir ingrese \"/\" : " )
            if word == "/":
                return 0
            articles = pd.read_csv("articles1.csv",usecols=[1,2,9])
            articles1 = pd.read_csv("articles2.csv",usecols=[1,2,9])
            articles2 = pd.read_csv("articles3.csv",usecols=[1,2,9])
            articlesFinal = pd.concat([articles, articles1, articles2])
            articlesFinal["frec"] = articlesFinal["content"].str.count(word.lower()) + articlesFinal["content"].str.count(word.upper()) + articlesFinal["content"].str.count(word.capitalize())
            articlesFinal = articlesFinal.sort_values(by='frec',ascending=False)
            print(articlesFinal.iloc[0:10,[3,0,1]])
    except:
        print("ERROR")
        counting()
text_analysis()