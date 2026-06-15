from bs4 import BeautifulSoup
import re
import requests


if __name__ == '__main__':
    # 预处理
    def pre_process():
        name = input("please put in the person's name who you want to search\n")
        url = 'https://baike.baidu.com/item/'+ name
        headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/112.0.0.0 Safari/537.36 Edg/112.0.1722.48',
        }
        cookies = {
            'BIDUPSID':'ECE53D27552A262909AE1AD3B2A261A8',
            'PSTM':'1673444977',
            'ab_jid':'800a0cadd1b25a839b00eff65d2d4ae9b134',
            'ab_jid_BFESS':'800a0cadd1b25a839b00eff65d2d4ae9b134',
            '__bid_n':'18669da4a119648a23c495',
            'ZFY':'L54qHzUJb25R:A7D6yy4WApVCa3hw1EEgBH7rgMe2Y4E:C',
            'newlogin':'1',
            'BAIDUID':'CB3D294E78B867CA647893E7FEA39C1D:FG=1',
            'BAIDUID_BFESS':'CB3D294E78B867CA647893E7FEA39C1D:FG=1',
            'BDUSS':'lNqV25heDVEZ2p5dHlxM3Q0VkhremU1QnFpajdPRWxEMEhJdURmczBDRjg5VnBrRVFBQUFBJCQAAAAAAAAAAAEAAADmZDCRztK1xMPOsKHO0tf21vcAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHxoM2R8aDNkN',
            'BDUSS_BFESS':'lNqV25heDVEZ2p5dHlxM3Q0VkhremU1QnFpajdPRWxEMEhJdURmczBDRjg5VnBrRVFBQUFBJCQAAAAAAAAAAAEAAADmZDCRztK1xMPOsKHO0tf21vcAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHxoM2R8aDNkN',
            'MCITY':'-:',
            'BDRCVFR[feWj1Vr5u3D]':'I67x6TjHwwYf0',
            'delPer':'0',
            'PSINO':'7',
            'H_PS_PSSID':'38516_36559_38470_38345_38468_38377_38485_37930_38414_38387_38356_26350_38186',
            'RT':'"z=1&dm=baidu.com&si=8fb893d2-3fb5-413a-b287-e9199821df6b&ss=lglzu1ya&sl=s&tt=1yp4&bcn=https://fclog.baidu.com/log/weirwood?type=perf"',
            'ab_bid':'0a71415efdc548c85254f619faa2cbe34938',
            'ab_sr':'1.0.1_ZGE5NjNkYzFkNTYzNDExODVkMzEwNTgxM2NkMjhkM2JjNjVmZGMxYzA3ZDBiNTAwZWMyNzBhNmMyNjk3ODAxMjExZjMxNmFiNTExZGNhYjIxYTlkNzlkN2NlMWRlYzQ4M2QzNzljMTI0NWJjYmMyYjFiOTFmOWZiODFhNmZiYTU4OWMzYjEwMTI5YmU0MmQ3ZmU5MDc5YWNkYmU0ZThmMmYxMTE5NzdmYTQ5ODE4NDVmMDY2ZWNlZjczM2RjYTQy'
        }
        response = requests.get(url=url,headers=headers,cookies=cookies)
        response.encoding = 'utf-8'
        content= response.text.replace('\n','')
        # 多义词
        # pos = content.find("lemmaWgt-subLemmaListTitle")
        # if(pos != -1):
        #     print("sorry,there are many people who has the same name")
        
        # 用正则表达式删除[],
        content = re.sub(r'\[\d+-\d+\]|\[\d+\]','',content)
        with open('原始文本.txt','w',encoding='utf-8') as fp:
            fp.write(content)
        return name,content

    # 得到soup
    def pre_soup(content):
        soup = BeautifulSoup(content,'html.parser')
        return soup

    # 人物简介
    def basic_infomation(soup):
        info = soup.find(class_ = 'lemma-summary J-summary')
        with open('人物简介.txt','w',encoding='utf-8') as fp:
            for i in info:
                fp.write(i.get_text())

    # 人物生平
    def basic_experience(soup):
        mess = soup.find(class_ = 'anchor-list')
        mess = mess.next_siblings
        with open('人物履历.txt','w',encoding='utf-8') as fp:
            for i in mess:
                text = i.get_text()
                text_pos = text.find("参考资料")
                if(text_pos != -1):
                    text = text[0:text_pos]
                text_pos = text.find("词条图册")
                if(text_pos != -1):
                    text = text[0:text_pos]
                fp.write(text)

    # 人物关系
    def basic_relation(name):
        p = soup.find(id="J-vars")
        id = p.attrs['data-lemmaid']
        headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.0.0 Safari/537.36 Edg/111.0.1661.44'}
        url = "https://baike.baidu.com/starmap/api/gethumanrelationcard?lemmaId="+id+"&lemmaTitle="+name
        content = requests.get(url,headers = headers)
        relation = content.json()['list']
        with open('人物关系.txt','w',encoding='utf-8') as fp:
            for i in relation:
                fp.write(i['relationName']+'\n')
                fp.write(i['lemmaTitle']+'\n')

    # 人物基本信息
    def basic_message():
        item = re.findall('<dt class="basicInfo-item name" id="basic-name">(.*?)</dt>',content)
        temp = []
        for i in item:
            i = i.replace('&nbsp','').replace(';','')
            temp.append(i)
        item = temp
        mess = re.findall('<dd class="basicInfo-item value">(.*?)</dd>',content)
        temp = []
        for i in mess:
            i = re.sub('<a(.*?)>','',i)
            i = re.sub('</a>','',i)
            i = re.sub('<br/>(.*?)<br/>','',i)
            i = re.sub('<sup(.*?)</sup>&nbsp;',',',i)
            i = re.sub('<em(.*?)em>',',',i)
            i = re.sub('<div(.*?)收起',',',i)
            i = re.sub('展开','',i)
            temp.append(i)
        mess = temp

        with open('基本信息.txt','w',encoding='utf-8') as fp:
            for i,j in zip(item,mess):
                t = i + ':' + j + '\n'
                fp.write(t)

    name,content = pre_process()
    soup = pre_soup(content)
    basic_infomation(soup)
    basic_experience(soup)
    basic_message()
    basic_relation(name)