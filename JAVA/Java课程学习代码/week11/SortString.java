package com.cl.week11;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.Scanner;

public class SortString {
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		ArrayList<String> strList = new ArrayList<String>();
		for(int i=0;i<4;i++) {
			strList.add(in.nextLine());
		}
		in.close();
		String[] str = new String[strList.size()];
		for(int i=0;i<strList.size();i++) {
			str[i] = strList.get(i);
		}
		Arrays.sort(str,new Comparator<String>() {
			@Override
			public int compare(String o1,String o2) {
				return o1.compareTo(o2);
			}
		});
		
		System.out.println(Arrays.toString(str));
		
	}
}
