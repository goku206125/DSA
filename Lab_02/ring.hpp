#include <stdexcept>
#include <iostream>

template <typename Key, typename Info>
class bi_ring
{
public:
    // struct representing each element of the bi_ring
    struct Element
    {
        Key key;
        Info info;
    };

private:
    // Node structure
    struct Node
    {
        Node(const Key &key, const Info &info, Node *prev, Node *next)
        {
            this->element.key = key;
            this->element.info = info;
            this->prev = prev;
            this->next = next;
        }
        Element element;
        Node *prev;
        Node *next;
    };
    Node *head;       // head node of the bi_ring
    unsigned int len; // number of elements in the bi_ring

public:
    // iterator for the bi_ring
    class iterator
    {
    private:
        // current node of the iterator
        Node *node;

    public:
        // make bi_ring parent class friend to the iterator
        friend class bi_ring;
        iterator(Node *node = nullptr) // constructor
        {
            this->node = node;
        }
        iterator &operator=(const iterator &src) // assignment operator
        {
            this->node = src.node;
            return *this;
        }
        bool operator==(const iterator &rhs) const // equality operator
        {
            return node == rhs.node;
        }
        bool operator!=(const iterator &rhs) const // not equal operator
        {
            return node != rhs.node;
        }
        iterator operator+(int pos) // addition operator
        {
            iterator i(node);
            while (pos)
            {
                i++;
                pos--;
            }
            return i;
        }
        iterator operator-(int pos) // subtraction operator
        {
            iterator i(node);
            while (pos)
            {
                i--;
                pos--;
            }
            return i;
        }
        iterator &operator++() // prefix increment operator
        {
            if (node)
                node = node->next;
            return *this;
        }
        iterator operator++(int) // postfix increment operator
        {
            iterator it = *this;
            ++*this;
            return it;
        }
        iterator &operator--() // prefix decrement operator
        {
            if (node)
                node = node->prev;
            return *this;
        }
        iterator operator--(int) // postfix decrement operator
        {
            if (node)
                return iterator(node->prev);
            return iterator(nullptr);
        }
        Element &operator*() // dereference operator
        {
            if (node)
                return node->element;
            throw std::runtime_error("Iterator points to nullptr");
        }
        Element *operator->() // arrow operator
        {
            if (node)
                return &node->element;
            throw std::runtime_error("Iterator points to nullptr");
        }
    };
    // constant iterator for bi_ring
    class const_iterator
    {
    private:
        Node *node; // current node of the iterator

    public:
        friend class bi_ring;
        const_iterator(Node *node) // constructor
        {
            this->node = node;
        }
        const_iterator &operator=(const const_iterator &src) // assignment operator
        {
            this->node = src.node;
            return *this;
        }
        bool operator==(const const_iterator &rhs) const // equality operator
        {
            return node == rhs.node;
        }
        bool operator!=(const const_iterator &rhs) const // not equal operator
        {
            return node != rhs.node;
        }
        const_iterator operator+(int pos) // addition operator
        {
            const_iterator i(node);
            while (pos)
            {
                i++;
                pos--;
            }
            return i;
        }
        const_iterator operator-(int pos) // subtraction operator
        {
            const_iterator i(node);
            while (pos)
            {
                i--;
                pos--;
            }
            return i;
        }
        const_iterator &operator++() // prefix increment operator
        {
            if (node)
                node = node->next;
            return *this;
        }
        const_iterator operator++(int) // postfix increment operator
        {
            const_iterator it = *this;
            ++*this;
            return it;
        }
        const_iterator &operator--() // prefix decrement operator
        {
            if (node)
                node = node->prev;
            return *this;
        }
        const_iterator operator--(int) // postfix decrement operator
        {
            if (node)
                return const_iterator(node->prev);
            return const_iterator(nullptr);
        }
        const Element &operator*() const // dereference operator
        {
            if (node)
                return node->element;
            throw std::runtime_error("Iterator points to nullptr");
        }
        const Element *operator->() const // arrow operator
        {
            if (node)
                return &node->element;
            throw std::runtime_error("Iterator points to nullptr");
        }
    };
    bi_ring() // constructor
    {
        head = nullptr;
        len = 0;
    }
    bi_ring(const bi_ring &src) // copy constructor
    {
        head = nullptr;
        len = 0;
        *this = src;
    }
    ~bi_ring() // destructor
    {
        clear();
    }
    bi_ring &operator=(const bi_ring &src) // assignment operator
    {
        if (this == &src)
        {
            return *this;
        }
        clear();
        const_iterator it = src.cbegin();
        for (unsigned int i = 0; i < src.len; i++, it++)
            push_back(it->key, it->info);
        return *this;
    }
    iterator push_front(const Key &key, const Info &info) // adds element with given key and info to the front and returns iterator to the element added
    {
        return insert(begin(), key, info);
    }
    iterator pop_front() // removes element from the front and returns iterator to the first element
    {
        return erase(begin());
    }
    iterator push_back(const Key &key, const Info &info) // adds element with given key and info to the back and returns iterator to the element added
    {
        return insert_after(end(), key, info);
    }
    iterator pop_back() // removes element from the front and returns iterator to the last element
    {
        return erase(end());
    }
    iterator insert(iterator position, const Key &key, const Info &info) // adds element with given key and info before the given position and returns iterator to the element added
    {
        if (head == nullptr and position.node == head)
        {
            head = new Node(key, info, nullptr, nullptr);
            head->next = head;
            head->prev = head;
            len++;
            return begin();
        }

        if (position.node)
        {
            Node *newNode = new Node(key, info, position.node->prev, position.node);
            position.node->prev->next = newNode;
            position.node->prev = newNode;
            len++;
            if (position.node == head)
            {
                head = newNode;
                return begin();
            }
            return iterator(newNode);
        }
        return position;
    }
    iterator insert_after(iterator position, const Key &key, const Info &info) // adds element with given key and info after the given position and returns iterator to the element added
    {
        if (head == nullptr and position.node == head)
        {
            head = new Node(key, info, nullptr, nullptr);
            head->next = head;
            head->prev = head;
            len++;
            return begin();
        }

        if (position.node)
        {
            Node *newNode = new Node(key, info, position.node, position.node->next);
            position.node->next->prev = newNode;
            position.node->next = newNode;
            len++;
            return iterator(newNode);
        }
        return position;
    }
    iterator erase(iterator position) // removes element at the given position and returns iterator to the element removed
    {
        if (len == 1 and position.node == head)
        {
            delete head;
            head = nullptr;
            return begin();
        }
        if (position.node)
        {
            Node *p = position.node->prev;
            Node *n = position.node->next;
            p->next = n;
            n->prev = p;
            len--;
            delete position.node;
            if (position.node == head)
            {
                head = n;
                return begin();
            }
        }
        return iterator(nullptr);
    }
    iterator find(const Key &key) // returns the iterator with given key(first occurance)
    {
        Node *node = head;
        while (node)
        {
            if (node->element.key == key)
                return iterator(node);
            node = node->next;
        }
        return iterator(nullptr);
    }
    bool empty() const // returns true if the bi_ring is empty else returns false
    {
        return head == nullptr;
    }
    unsigned int size() const // returns the number of elements in the bi_ring
    {
        return len;
    }
    void clear() // removes all the elements of the bi_ring
    {
        while (!empty())
            pop_front();
        head = nullptr;
        len = 0;
    }
    void print() const // displays all the elements of the bi_ring
    {
        const_iterator it = cbegin();
        for (unsigned int i = 0; i < len; i++, it++)
            std::cout << it->key << ": " << it->info << "\n";
    }
    iterator begin() // returns an iterator to the first element
    {
        return iterator(head);
    }
    iterator end() // returns an iterator to the last element
    {
        return --iterator(head);
    }
    const_iterator cbegin() const // returns an const_iterator to the first element
    {
        return const_iterator(head);
    }
    const_iterator cend() const // returns an const_iterator to the last element
    {
        return --const_iterator(head);
    }
};

template <typename Key, typename Info>
void split(bi_ring<Key, Info> &source, bi_ring<Key, Info> &r1, int r1c, int r1r, bi_ring<Key, Info> &r2, int r2c, int r2r)
{
    auto it = source.begin();
    while (1)
    {
        for (int i = 0; i < r1c and r1r; i++, it++)
            r1.push_back(it->key, it->info);
        r1r--;
        for (int j = 0; j < r2c and r2r; j++, it++)
            r2.push_back(it->key, it->info);
        r2r--;
        if (r1r <= 0 and r2r <= 0)
            break;
    }
}