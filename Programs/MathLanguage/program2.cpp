void main()
{
    double a, b, c;

    input >> a >> b >> c;

    if (a == 0)
    {
        if ( b == 0 && c == 0)
            output << "����. ����� �������";
        else if (b == 0 && c != 0)
            output << "������� ���";
        output -c/b;
    }
    else
    {
        double D = b*b-4*a*c;

        if (d < 0)
            output << "������� ���";
        else if (d == 0)
        {
            output << -b/(2*a);
        }
        else
        {
            output << (-b + sqrt(D))/(2*a) <<  (-b - sqrt(D))/(2*a)
        }
    }
}