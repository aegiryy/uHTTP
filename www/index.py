from sys import argv

def parse_params(argv):
    '''return a param dict, everything is strings'''
    params = {}
    for i in range(1, len(argv)):
        k, v = argv[i].split('=')
        params[k] = v
    return params

GET = parse_params(argv)

print 'HTTP/1.1 200 OK'
print 'Content-Type: text/html'
print ''
print int(GET.get('a', 0)) + int(GET.get('b', 0))
