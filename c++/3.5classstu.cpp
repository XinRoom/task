#include<string>
#include<iostream>
using namespace std;
class student {
public:
	int mu = 3;
	student(string n, string nam, double M, double E, double C, double t, double ave);
	double sum();
	double average();
	double print();
	void get_reg_num();
	void set_stu_inf();
	void set_mu() { cin >> mu; };
	string seach(string n);
private:
	string num; string name; double Math; double English; double Computer; double summ; double averagee;
};
student::student(string n, string nam, double M, double E, double C, double t = 10.0, double ave = 10.0) {
	num = n;  name = nam;  English = E;   Math = M;   Computer = C;  summ = t;  averagee = ave;
}
student stu[3] = {
	student("02128","���",64,56,74),
	student("02120","����",51,63,84),
	student("02119","������",76,87,56),
};
double student::sum() { return(English + Math + Computer); }
double student::average() { return(sum() / 3.0); }
double student::print() {
	cout << "ѧ��" << "\t" << "����" << "\t" << "Ӣ��" << "\t" << "��ѧ" << "\t" << "�����" << "\t" << "�ܷ�" << "\t" << "ƽ����" << endl;
	cout << num << "\t" << name << "\t" << Math << "\t" << English << "\t" << Computer << "\t" << sum() << "\t" << average() << endl;
	return 0;
}
void student::get_reg_num() {
	string inname;
	cin >> inname;
	for (int i = 0; i < mu; i++) {
		if (stu[i].name == inname)
		{
			cout << stu[i].num << endl;
		}
	}
}
void student::set_stu_inf() {
	string n, nam; double M, E, C;
	for (int i = 0; i<mu; i++) {
		cin >> n >> nam >> E >> M >> C;
		stu[i].num = n;
		stu[i].name = nam;
		stu[i].English = E;
		stu[i].Math = M;
		stu[i].Computer = C;
	}
}
string student::seach(string nn) {
	for (int i = 0; i < mu; i++) {
		if (stu[i].num == nn)
		{
			stu[i].print();
		}
	}
	return 0;
}
void main() {
	string nn;
	cout << "��Ҫ�����������Ϣ��:";
	stu[0].set_mu();
	cout << "��������ѧ����Ϣ:(��ʽ:ѧ��[�س�]����[�س�]Ӣ��[�س�]��ѧ[�س�]�����[�س�])" << endl;
	stu[0].set_stu_inf();
	for (int i = 0; i<stu[0].mu; i++) {
		stu[i].print();
	}
	cout << "���������ͨ��������ѧ�ţ�";
	stu[0].get_reg_num();
	cout << "���������ͨ��ѧ���ҵ���ѧ����ȫ����Ϣ��";
	cin >> nn;
	stu[0].seach(nn);
	system("pause");
}
//�������쳣: 0xC0000005 : ��ȡλ�� 0x00000000 ʱ�������ʳ�ͻ��