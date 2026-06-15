from bs4 import BeautifulSoup
import re
import urllib.parse
import urllib.request
import json
import jsonpath
from lxml import etree

def get_url(name):
    return 'https://baike.baidu.com/item/' + urllib.parse.quote(name)

def pre_process(name,url,q):
    found = 0 # 是否找到
    headers = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/112.0.0.0 Safari/537.36 Edg/112.0.1722.48',
        'Cookie':'BIDUPSID=ECE53D27552A262909AE1AD3B2A261A8; PSTM=1673444977; __bid_n=18669da4a119648a23c495; ZFY=L54qHzUJb25R:A7D6yy4WApVCa3hw1EEgBH7rgMe2Y4E:C; newlogin=1; BAIDUID=CB3D294E78B867CA647893E7FEA39C1D:FG=1; BAIDUID_BFESS=CB3D294E78B867CA647893E7FEA39C1D:FG=1; BDUSS=lNqV25heDVEZ2p5dHlxM3Q0VkhremU1QnFpajdPRWxEMEhJdURmczBDRjg5VnBrRVFBQUFBJCQAAAAAAAAAAAEAAADmZDCRztK1xMPOsKHO0tf21vcAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHxoM2R8aDNkN; BDUSS_BFESS=lNqV25heDVEZ2p5dHlxM3Q0VkhremU1QnFpajdPRWxEMEhJdURmczBDRjg5VnBrRVFBQUFBJCQAAAAAAAAAAAEAAADmZDCRztK1xMPOsKHO0tf21vcAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHxoM2R8aDNkN; MCITY=-:; BA_HECTOR=a0a0058l8l01800l01852ke91i3tot61m; BDORZ=B490B5EBF6F3CD402E515D22BCDA1598; channel=bing; BK_SEARCHLOG={"key":["李亚轩","龙文章","龙文章我的团","沈从文","李白","人物","体育人物","历史人物体育人物","硪维度大发给","剑"]}; X_ST_FLOW=0; Hm_lvt_55b574651fcae74b0a9f1cf9c8d7c93a=1681843122,1681843636,1681869634,1681887405; Hm_lpvt_55b574651fcae74b0a9f1cf9c8d7c93a=1681887412; H_PS_PSSID=38516_36559_38470_38345_38468_38377_38485_37930_38414_38387_38356_26350_38186; zhishiTopicRequestTime=1681887662692; ab_sr=1.0.1_MmQ1MGRmNzcyN2RjNmNjMDNiMThjNzg5ZGMwNzNhZmM5OWMzMWM2ZmI0NDE5OWEyNzU2NTlmNzIzZTlhZjA0N2Y4ZjA2MmI5ODMwZjA5MmQ1MTQ1ZmM4MzQ2OTRiN2NiOTdjYjFjNzBhNDgzZjhjYmM1NmJhOTVhNzNkYjJjNWRjMzAzN2JiZDU4MTM5NmNjMGE4NzYxMWUxN2YyMzhlZWQ2ODU1MTFiNjMzYzNhMDQ4NjkzOTczZDAwNGE5YTFl; RT="z=1&dm=baidu.com&si=8fb893d2-3fb5-413a-b287-e9199821df6b&ss=lgnbea3y&sl=o&tt=63l&bcn=https://fclog.baidu.com/log/weirwood?type=perf"; baikeVisitId=044fe712-1133-4fb5-8305-ce4d6f8a7d44'
    }    
    # 请求头包含User-Agent，反爬
    url = url.encode('utf-8').decode('latin1')
    headers['Cookie'] = headers['Cookie'].encode('utf-8').decode('latin1')
    headers['User-Agent'] = headers['User-Agent'].encode('utf-8').decode('latin1')
    request = urllib.request.Request(url=url, headers=headers)
    response = urllib.request.urlopen(request)
    content = response.read().decode('utf-8').replace('\'','‘').replace('\n','  ') # sql用
    content = re.sub(r'\[\d+-\d+\]|\[\d+\]','',content) # 删引用
    soup = BeautifulSoup(content,'html.parser')
    
    pos = content.find("lemmaWgt-subLemmaListTitle")
    if(pos != -1):
        found = 1
        # 把网址放进去
        mess = re.findall('data-lemmaid=(\d+)>',content)
        id_list = set(())
        for m in mess :
            id_list.update([m])
        id_list = list(id_list)
        for i in id_list:
            i=url+'/'+i
            q.put(i)
    return name,content,soup,q,found

def pre_process_weak(name,url):
    headers = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/112.0.0.0 Safari/537.36 Edg/112.0.1722.48',
        'Cookie':'BIDUPSID=ECE53D27552A262909AE1AD3B2A261A8; PSTM=1673444977; __bid_n=18669da4a119648a23c495; ZFY=L54qHzUJb25R:A7D6yy4WApVCa3hw1EEgBH7rgMe2Y4E:C; newlogin=1; BAIDUID=CB3D294E78B867CA647893E7FEA39C1D:FG=1; BAIDUID_BFESS=CB3D294E78B867CA647893E7FEA39C1D:FG=1; BDUSS=lNqV25heDVEZ2p5dHlxM3Q0VkhremU1QnFpajdPRWxEMEhJdURmczBDRjg5VnBrRVFBQUFBJCQAAAAAAAAAAAEAAADmZDCRztK1xMPOsKHO0tf21vcAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHxoM2R8aDNkN; BDUSS_BFESS=lNqV25heDVEZ2p5dHlxM3Q0VkhremU1QnFpajdPRWxEMEhJdURmczBDRjg5VnBrRVFBQUFBJCQAAAAAAAAAAAEAAADmZDCRztK1xMPOsKHO0tf21vcAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHxoM2R8aDNkN; MCITY=-:; BA_HECTOR=a0a0058l8l01800l01852ke91i3tot61m; BDORZ=B490B5EBF6F3CD402E515D22BCDA1598; channel=bing; BK_SEARCHLOG={"key":["李亚轩","龙文章","龙文章我的团","沈从文","李白","人物","体育人物","历史人物体育人物","硪维度大发给","剑"]}; X_ST_FLOW=0; Hm_lvt_55b574651fcae74b0a9f1cf9c8d7c93a=1681843122,1681843636,1681869634,1681887405; Hm_lpvt_55b574651fcae74b0a9f1cf9c8d7c93a=1681887412; H_PS_PSSID=38516_36559_38470_38345_38468_38377_38485_37930_38414_38387_38356_26350_38186; zhishiTopicRequestTime=1681887662692; ab_sr=1.0.1_MmQ1MGRmNzcyN2RjNmNjMDNiMThjNzg5ZGMwNzNhZmM5OWMzMWM2ZmI0NDE5OWEyNzU2NTlmNzIzZTlhZjA0N2Y4ZjA2MmI5ODMwZjA5MmQ1MTQ1ZmM4MzQ2OTRiN2NiOTdjYjFjNzBhNDgzZjhjYmM1NmJhOTVhNzNkYjJjNWRjMzAzN2JiZDU4MTM5NmNjMGE4NzYxMWUxN2YyMzhlZWQ2ODU1MTFiNjMzYzNhMDQ4NjkzOTczZDAwNGE5YTFl; RT="z=1&dm=baidu.com&si=8fb893d2-3fb5-413a-b287-e9199821df6b&ss=lgnbea3y&sl=o&tt=63l&bcn=https://fclog.baidu.com/log/weirwood?type=perf"; baikeVisitId=044fe712-1133-4fb5-8305-ce4d6f8a7d44'
    }    
    # 请求头包含User-Agent，反爬
    url = url.encode('utf-8').decode('latin1')
    headers['Cookie'] = headers['Cookie'].encode('utf-8').decode('latin1')
    headers['User-Agent'] = headers['User-Agent'].encode('utf-8').decode('latin1')
    request = urllib.request.Request(url=url, headers=headers)
    response = urllib.request.urlopen(request)
    content = response.read().decode('utf-8').replace('\'','‘').replace('\n','  ') # sql用
    content = re.sub(r'\[\d+-\d+\]|\[\d+\]','',content) # 删引用
    soup = BeautifulSoup(content,'html.parser')
    return name,content,soup

def get_id(content):     #获取每个人物对应的唯一id
    html_tree=etree.HTML(content)   #利用xpath解析
    id=html_tree.xpath('//div[@style="display:none;"]//@data-lemmaid')   #找到合适的xpath路径，返回一个列表
    return str(id[0])

# 人物简介
def basic_infomation(soup):
    info = soup.find(class_ = 'lemma-summary J-summary')
    information = ''
    if info == None:
        exist_the_people = 0
    else:
        exist_the_people = 1
    
    if exist_the_people == 0:
        return information,exist_the_people
    
    for i in info:
        information = information + i.get_text()
    return information,exist_the_people

# 人物基本信息
def basic_message(content):
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

    message = ''
    for i,j in zip(item,mess):
        t = i + ':' + j + '\n'
        message = message + t
    return message

# 人物生平
def basic_experience(soup):
    mess = soup.find(class_ = 'anchor-list')
    experience = ''
    if mess == None:
        return experience
    mess = mess.next_siblings

    for i in mess:
        text = i.get_text()
        text_pos = text.find("参考资料")
        if(text_pos != -1):
            text = text[0:text_pos]
            break
        text_pos = text.find("词条图册")
        if(text_pos != -1):
            text = text[0:text_pos]
            break
        experience = experience + text
    return experience
        
def basic_relationship(content,name):
    url = 'https://baike.baidu.com/starmap/api/gethumanrelationcard?'  #基础的url，无具体参数
    # id 和 name
    data = {                                                                 
        'lemmaId': get_id(content),
        'lemmaTitle': name
    }
    url = url + urllib.parse.urlencode(data)  #利用urlencode拼接合成完整的url
    headers = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/112.0.0.0 Safari/537.36 Edg/112.0.1722.48',
        'Cookie':'BIDUPSID=ECE53D27552A262909AE1AD3B2A261A8; PSTM=1673444977; __bid_n=18669da4a119648a23c495; ZFY=L54qHzUJb25R:A7D6yy4WApVCa3hw1EEgBH7rgMe2Y4E:C; newlogin=1; BAIDUID=CB3D294E78B867CA647893E7FEA39C1D:FG=1; BAIDUID_BFESS=CB3D294E78B867CA647893E7FEA39C1D:FG=1; BDUSS=lNqV25heDVEZ2p5dHlxM3Q0VkhremU1QnFpajdPRWxEMEhJdURmczBDRjg5VnBrRVFBQUFBJCQAAAAAAAAAAAEAAADmZDCRztK1xMPOsKHO0tf21vcAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHxoM2R8aDNkN; BDUSS_BFESS=lNqV25heDVEZ2p5dHlxM3Q0VkhremU1QnFpajdPRWxEMEhJdURmczBDRjg5VnBrRVFBQUFBJCQAAAAAAAAAAAEAAADmZDCRztK1xMPOsKHO0tf21vcAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHxoM2R8aDNkN; MCITY=-:; BA_HECTOR=a0a0058l8l01800l01852ke91i3tot61m; BDORZ=B490B5EBF6F3CD402E515D22BCDA1598; channel=bing; BK_SEARCHLOG={"key":["李亚轩","龙文章","龙文章我的团","沈从文","李白","人物","体育人物","历史人物体育人物","硪维度大发给","剑"]}; X_ST_FLOW=0; Hm_lvt_55b574651fcae74b0a9f1cf9c8d7c93a=1681843122,1681843636,1681869634,1681887405; Hm_lpvt_55b574651fcae74b0a9f1cf9c8d7c93a=1681887412; H_PS_PSSID=38516_36559_38470_38345_38468_38377_38485_37930_38414_38387_38356_26350_38186; zhishiTopicRequestTime=1681887662692; ab_sr=1.0.1_MmQ1MGRmNzcyN2RjNmNjMDNiMThjNzg5ZGMwNzNhZmM5OWMzMWM2ZmI0NDE5OWEyNzU2NTlmNzIzZTlhZjA0N2Y4ZjA2MmI5ODMwZjA5MmQ1MTQ1ZmM4MzQ2OTRiN2NiOTdjYjFjNzBhNDgzZjhjYmM1NmJhOTVhNzNkYjJjNWRjMzAzN2JiZDU4MTM5NmNjMGE4NzYxMWUxN2YyMzhlZWQ2ODU1MTFiNjMzYzNhMDQ4NjkzOTczZDAwNGE5YTFl; RT="z=1&dm=baidu.com&si=8fb893d2-3fb5-413a-b287-e9199821df6b&ss=lgnbea3y&sl=o&tt=63l&bcn=https://fclog.baidu.com/log/weirwood?type=perf"; baikeVisitId=044fe712-1133-4fb5-8305-ce4d6f8a7d44'
    }
    url = url.encode('utf-8').decode('latin1')
    headers['Cookie'] = headers['Cookie'].encode('utf-8').decode('latin1')
    headers['User-Agent'] = headers['User-Agent'].encode('utf-8').decode('latin1')
    request = urllib.request.Request(url=url, headers=headers)   #定制请求对象
    content = urllib.request.urlopen(request).read().decode('utf-8')  #获取服务器返回的数据并解码，获取文本内容
    with open('temp_relation.json', 'w', encoding='utf-8') as fs: #将爬取到的内容存入本地文件中
        fs.write(content)
    obj = json.load(open('temp_relation.json', 'r', encoding='utf-8'))  #采用Jsonpath解析方式
    relation_name = jsonpath.jsonpath(obj, '$..relationName')   #确定合适的Jsonpath解析路径
    hisname = jsonpath.jsonpath(obj, '$..lemmaTitle')
    res=''
    if relation_name == 0:# 为空，提前结束
        return ''
    for i, j in zip(relation_name, hisname):#按对应的顺序输出两个列表中的值
        res = res + i + ' : ' + j + ', '
    return res.strip(', ')