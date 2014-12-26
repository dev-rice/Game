import random
import sys

other_events = ["The Zerg Swarms finally conquer Texas.",
                "Humans discover Middle Earth.",
                "The 2048 bit computer is invented.",
                "The second coming of the Christ Jesus occurs. There were no survivors.",
                "Octopi, having rode in the evolutionary back seat until now, complete the conquest of space."]

def getListItemsFromList(list_item):
    list_items = list_item.findAll('li')
    list_contents = []
    for item in list_items:
        sublists = item.findAll('ul')
        if len(sublists) != 0:
            for list in sublists:
                contents = getListItemsFromList(list)
                for content in contents:
                    list_contents.append(content)
        else:
            list_contents.append(item.text + " ")

    return list_contents

def getRandomEvent():
    
    return random.choice(other_events)

if len(sys.argv) == 2:
    year = int(sys.argv[1])
    random_event = ""
    try:
        import urllib2
        from BeautifulSoup import BeautifulSoup 
        if (year < 2600):
            response = urllib2.urlopen('http://en.wikipedia.org/wiki/' + str(year))
            html = response.read()

            soup = BeautifulSoup(html)

            headers = soup.findAll('ul')
            random_header = random.choice(headers)

            list_contents = getListItemsFromList(random_header)
            random_event = random.choice(list_contents)
        else:
            random_event = getRandomEvent()
    except (IndexError, ImportError):
        random_event = getRandomEvent()

    print "In the year \033[94m%d\033[0m: %s\n" % (year, random_event)
