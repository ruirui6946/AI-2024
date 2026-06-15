#本文件用来获取人物信息
from bs4 import BeautifulSoup
import re
import urllib.parse
import urllib.request
import json
import jsonpath
from lxml import etree

def get_url(name):
    return 'https://baike.baidu.com/item/' + urllib.parse.quote(name)

def pre_process(url):
    headers = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/112.0.0.0 Safari/537.36 Edg/112.0.1722.48',
        'Cookie':'channel=bing; BAIDUID=8DCFADB2B96CE515930EA3927DCCF95E:FG=1; BAIDUID_BFESS=8DCFADB2B96CE515930EA3927DCCF95E:FG=1; log_first_time=1683554831784; log_last_time=1683554856232; zhishiTopicRequestTime=1683790901495; Hm_lvt_55b574651fcae74b0a9f1cf9c8d7c93a=1683529271,1683790910; X_ST_FLOW=0; baikeVisitId=afb04da6-9b00-4dfd-bfe5-f4903fcb9132; BK_SEARCHLOG={"key":["李白","毛泽东"]}; Hm_lpvt_55b574651fcae74b0a9f1cf9c8d7c93a=1683791712; RT="z=1&dm=baidu.com&si=4874634d-d6ad-4e96-ad8e-6271c82675ae&ss=lhitml54&sl=1a&tt=nhm&bcn=https://fclog.baidu.com/log/weirwood?type=perf"; ab_sr=1.0.1_NzY5MjM4ZTg0NTM2MDgwZWYzYTU5ZWM3NjBiODJiOTFiYjQ2YWE0YmI2OWQwYTVlNzJjMTZiNDU3MjJjYjIyZDc4NDNjNThmNTJjNmNjNTIyY2JhMGNhMGZmYmJmMGZhM2Y2ZGE3MjI2MjhiYjBiZjQ4YWNlZWE2ODViMjM4ZTVmNWE4M2M1ODY2ZDA5ZDk2Y2NkODI2NDM5NmEyNTBhMQ=='
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
    return content,soup

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
        
def basic_relationship(content,name,q):
    url = 'https://baike.baidu.com/starmap/api/gethumanrelationcard?'  #基础的url，无具体参数
    # id 和 name
    data = {                                                                 
        'lemmaId': get_id(content),
        'lemmaTitle': name
    }
    url = url + urllib.parse.urlencode(data)  #利用urlencode拼接合成完整的url
    headers = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/112.0.0.0 Safari/537.36 Edg/112.0.1722.48',
        'Cookie':'channel=bing; BAIDUID=8DCFADB2B96CE515930EA3927DCCF95E:FG=1; BAIDUID_BFESS=8DCFADB2B96CE515930EA3927DCCF95E:FG=1; log_first_time=1683554831784; log_last_time=1683554856232; zhishiTopicRequestTime=1683790901495; Hm_lvt_55b574651fcae74b0a9f1cf9c8d7c93a=1683529271,1683790910; X_ST_FLOW=0; baikeVisitId=afb04da6-9b00-4dfd-bfe5-f4903fcb9132; BK_SEARCHLOG={"key":["李白","毛泽东"]}; Hm_lpvt_55b574651fcae74b0a9f1cf9c8d7c93a=1683791712; RT="z=1&dm=baidu.com&si=4874634d-d6ad-4e96-ad8e-6271c82675ae&ss=lhitml54&sl=1a&tt=nhm&bcn=https://fclog.baidu.com/log/weirwood?type=perf"; ab_sr=1.0.1_NzY5MjM4ZTg0NTM2MDgwZWYzYTU5ZWM3NjBiODJiOTFiYjQ2YWE0YmI2OWQwYTVlNzJjMTZiNDU3MjJjYjIyZDc4NDNjNThmNTJjNmNjNTIyY2JhMGNhMGZmYmJmMGZhM2Y2ZGE3MjI2MjhiYjBiZjQ4YWNlZWE2ODViMjM4ZTVmNWE4M2M1ODY2ZDA5ZDk2Y2NkODI2NDM5NmEyNTBhMQ=='
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
        q.put([j,get_url(j)])
        res = res + 'name:'+j + 'relationship name:' + i + '\n'
    return res.strip()