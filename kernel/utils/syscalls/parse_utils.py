def find_block(text, pos, parentheses):
    opened = False
    balance = 0

    begin = pos
    for i in range(pos, len(text)):
        if text[i] == parentheses[0]:
            balance += 1
            if not opened:
                opened = True
                begin = i
        elif text[i] == parentheses[1]:
            balance -= 1
        if opened and balance == 0:
            return begin, i
        if balance < 0:
            raise ValueError
    raise ValueError
