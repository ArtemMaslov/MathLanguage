void main()
{
    double f = 1;
    int n;

    input >> n;
    if (n < 0)
        output << "��������� ����� �� ���������";

    for (int st = 1; st <= n; st++)
    {
        f *= st;
    }

    output << f;
}