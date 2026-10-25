extern int external_transform(int value);

int call_external(int value);

int call_external(int value)
{
    return external_transform(value) + 1;
}
