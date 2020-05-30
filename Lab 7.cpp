#include <iostream>
#include <fstream>

using namespace std;

struct newGraph
{
    int size;
    int size_of_matrix = 2;
    bool** matrix = new bool* [size_of_matrix];

    newGraph(int size)
    {
        for (int i = 0; i < size_of_matrix; i++)
        {
            matrix[i] = new bool[size_of_matrix];
        }
        clear();
        this->size = size;
        if (size > size_of_matrix) change_size_of_matrix(size / 2 * 3);
    }

    void change_size_of_matrix(int new_size)
    {
        bool** new_matrix = new bool* [new_size];

        for (int i = 0; i < new_size; i++)
        {
            new_matrix[i] = new bool[new_size];
            for (int j = 0; j < new_size; j++)
            {
                if (i < size && j < size) new_matrix[i][j] = matrix[i][j];
                else new_matrix[i][j] = false;
            }
        }
        swap(matrix, new_matrix);
        for (int i = 0; i < size; i++)
        {
            delete[] new_matrix[i];
        }
        size_of_matrix = new_size;
    }

    void add_edge(int first, int next)
    {
        int max;
        if (first > next) max = first;
        else max = next;
        if (max + 1 > size_of_matrix)
        {
            change_size_of_matrix((max + 1) / 2 * 3);
            size = max + 1;
        }
        matrix[first][next] = true;
    }

    void delete_edge(int first, int next)
    {
        if (first < size && next < size)
        {
            matrix[first][next] = false;
        }
    }

    void merge(int old, int in)
    {
        for (int i = 0; i < size; i++)
        {
            if (i != old)
            {
                if (matrix[i][old]) matrix[i][in] = true;
                if (matrix[old][i]) matrix[in][i] = true;
            }
        }
        if (old == size - 1)
        {
            for (int i = 0; i < size; i++)
            {
                matrix[i][old] = false;
            }
            for (int j = 0; j < size; j++)
            {
                matrix[old][j] = false;
            }
        }
        else
        {
            for (int i = old; i < size - 1; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    matrix[i][j] = matrix[i + 1][j];
                }
            }
            for (int i = 0; i < size; i++)
            {
                for (int j = old; j < size - 1; j++)
                {
                    matrix[i][j] = matrix[i][j + 1];
                }
            }
        }
        size--;
    }

    int* matrix_to_arr()
    {
        int size_of_arr = 0;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (matrix[i][j]) size_of_arr++;
            }
        }

        ++size_of_arr *= 2;

        int* arr = new int[size_of_arr];
        int index = 0;
        arr[index++] = size_of_arr;
        arr[index++] = size;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (matrix[i][j])
                {
                    arr[index++] = i;
                    arr[index++] = j;
                }
            }
        }
        return arr;
    }

    void arr_to_matrix(int* arr)
    {
        clear();
        int n = arr[0];
        for (int i = 2; i < n; i += 2)
        {
            add_edge(arr[i], arr[i + 1]);
        }
        delete[] arr;
    }

    void writeMatrix()
    {
        ofstream out;
        out.open("desired_file", ios::binary);
        int* arr = matrix_to_arr();
        int n = arr[0];
        if (out.is_open())
        {
            out.write((char*)&(*arr), sizeof(int) * n);
            out.close();
        }
        delete[] arr;
    }

    void readMatrix()
    {
        ifstream in;
        in.open("desired_file", ios::binary);
        if (in.is_open())
        {
            in.seekg(0, ios::end);
            int n = in.tellg() / 4;
            in.seekg(0, ios::beg);
            int* arr = new int[n];
            in.read((char*)&(*arr), sizeof(int) * n);
            arr_to_matrix(arr);
            in.close();
        }
    }

    void clear()
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                matrix[i][j] = false;
            }
        }
    }

    void print()
    {
        for (int j = 0; j < size; j++)
        {
            for (int i = 0; i < size; i++)
            {
                cout << matrix[i][j] << "\t";
            }
            cout << endl;
        }
        cout << endl;
    }
};
void supporting_matrix(bool** graph_matrix, int** to_matrix, int size_of_matrix) 
{
    for (int i = 0; i < size_of_matrix; i++)
    {
        to_matrix[i][0] = i;
        to_matrix[i][1] = 0;
    }

    for (int j = 0; j < size_of_matrix; j++) 
    {
        for (int i = 0; i < size_of_matrix; i++)
        {
            to_matrix[j][1] += graph_matrix[i][j];
        }
    }
}
bool** сonversely_supporting_matrix(int** to_matrix, bool** graph_matrix, int size_of_matrix)
{
    bool** result_matrix = new bool* [size_of_matrix];
    for (int i = 0; i < size_of_matrix; i++) 
    {
        result_matrix[i] = new bool[size_of_matrix];
        for (int j = 0; j < size_of_matrix; j++)
        {
            result_matrix[i][j] = false;
        }
    }
    for (int i = 0; i < size_of_matrix; i++) 
    {
        for (int j = 0; j < size_of_matrix; j++)
        {
            if (graph_matrix[i][j])
            {
                result_matrix[to_matrix[i][0]][to_matrix[j][0]] = true;
            }
        }
    }
    return result_matrix;
}

void sorting_by_inserts(int** to_matrix, int size_of_matrix)
{
    for (int i = 1; i < size_of_matrix; i++)
        for (int j = i; j > 0 && to_matrix[j - 1][1] > to_matrix[j][1]; j--)
        {
            swap(to_matrix[j - 1][0], to_matrix[j][0]);
            swap(to_matrix[j - 1][1], to_matrix[j][1]);
        }
}

void bubble_sort(int** to_matrix, int size_of_matrix)
{
    int a= 0;

    for (int k = 1; k < size_of_matrix; k++) 
    {
        for (int i = 0; i < size_of_matrix - k; i++)
        {
            if (to_matrix[i][1] > to_matrix[i + 1][1]) 
            {
                a = to_matrix[i][0];
                to_matrix[i][0] = to_matrix[i + 1][0];
                to_matrix[i + 1][0] = a;
                a = to_matrix[i][1];
                to_matrix[i][1] = to_matrix[i + 1][1];
                to_matrix[i + 1][1] = a;
            }
        }
    }
}

void shaker_sort(int** to_matrix, int size_of_matrix) 
{
    bool flag_r = true;
    int l = 1, r = 1;

    while (r + l != size_of_matrix)
    {
        if (flag_r) 
        {
            for (int i = 0; i < size_of_matrix - r; i++)
            {
                if (to_matrix[i][1] > to_matrix[i + 1][1]) 
                {
                    swap(to_matrix[i][0], to_matrix[i + 1][0]);
                    swap(to_matrix[i][1], to_matrix[i + 1][1]);
                }
            }
            r++;
            flag_r = false;
        }
        else {
            for (int i = size_of_matrix - l - 1; i > l; i--) 
            {
                if (to_matrix[i][1] < to_matrix[i - 1][1]) 
                {
                    swap(to_matrix[i][0], to_matrix[i - 1][0]);
                    swap(to_matrix[i][1], to_matrix[i - 1][1]);
                }
            }
            l++;
            flag_r = true;
        }
    }
}

void quick_sort(int** to_matrix, int l, int r) 
{
    int i = l, j = r;
    int temp = to_matrix[(l + r) / 2][1];

    while (i <= j)
    {
        while (to_matrix[i][1] < temp)
            i++;

        while (to_matrix[j][1] > temp)
            j--;

        if (i <= j)
        {
            swap(to_matrix[i][0], to_matrix[j][0]);
            swap(to_matrix[i][1], to_matrix[j][1]);

            i++;
            j--;
        }
    }
    if (l < j) quick_sort(to_matrix, l, j);
    if (i < r) quick_sort(to_matrix, i, r);
}

void merge(int** to_matrix, int l, int h, int m) {
    int i, j, k, c[50][2];
    i = l;
    k = l;
    j = m + 1;
    while (i <= m && j <= h) 
    {
        if (to_matrix[i][1] < to_matrix[j][1]) 
        {
            c[k][0] = to_matrix[i][0];
            c[k][1] = to_matrix[i][1];
            k++;
            i++;
        }
        else 
        {
            c[k][0] = to_matrix[j][0];
            c[k][1] = to_matrix[j][1];
            k++;
            j++;
        }
    }
    while (i <= m) 
    {
        c[k][0] = to_matrix[i][0];
        c[k][1] = to_matrix[i][1];
        k++;
        i++;
    }
    while (j <= h)
    {
        c[k][0] = to_matrix[j][0];
        c[k][1] = to_matrix[j][1];
        k++;
        j++;
    }
    for (i = l; i < k; i++) 
    {
        to_matrix[i][0] = c[i][0];
        to_matrix[i][1] = c[i][1];
    }
}

void merge_sort(int** to_matrix, int l, int h) 
{
    int m;
    if (l < h)
    {
        m = (l + h) / 2;
        merge_sort(to_matrix, l, m);
        merge_sort(to_matrix, m + 1, h);
        merge(to_matrix, l, h, m);
    }
}

void shell_sort(int** to_matrix, int size_of_matrix) 
{
    int next, i, j;
    int temp[2];

    for (next = size_of_matrix / 2; next > 0; next /= 2)
        for (i = next; i < size_of_matrix; i++)
            for (j = i - next; j >= 0 && to_matrix[j][1] > to_matrix[j + next][1]; j -= next) 
            {
                temp[0] = to_matrix[j][0];
                temp[1] = to_matrix[j][1];
                to_matrix[j][0] = to_matrix[j + next][0];
                to_matrix[j][1] = to_matrix[j + next][1];
                to_matrix[j + next][0] = temp[0];
                to_matrix[j + next][1] = temp[1];
            }
}


    
int main()
{
    newGraph new_created_graph(2);
    new_created_graph.add_edge(0, 3);
    new_created_graph.add_edge(3, 2);
    new_created_graph.add_edge(2, 2);
    new_created_graph.writeMatrix();
    new_created_graph.readMatrix();
    new_created_graph.print();
    int** to_matrix = new int* [new_created_graph.size];
    for (int i = 0; i < new_created_graph.size_of_matrix; i++)
    {
        to_matrix[i] = new int[2];
    }
    supporting_matrix(new_created_graph.matrix, to_matrix, new_created_graph.size);
    merge_sort(to_matrix, 0, new_created_graph.size - 1);
    new_created_graph.matrix = сonversely_supporting_matrix(to_matrix, new_created_graph.matrix, new_created_graph.size);
    new_created_graph.print();
}