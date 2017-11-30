package com.cl.week11;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Scanner;

public class WordsCount {
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		String str = new String(in.nextLine());
		in.close();
		HashMap<String,Integer> hm = new HashMap<String,Integer>();
		String tem = new String();
		for(int i=0;i<str.length();i++) {
			if(Character.isLetter(str.charAt(i))) {
				tem+=str.charAt(i);
			}
			else {
				if(hm.containsKey(tem)) {
					int cnt = hm.get(tem);
					cnt++;
					hm.put(tem, cnt);
				}
				else {
					System.out.println(tem);
					hm.put(tem,1);
				}
				tem = "";
			}
		}
		Iterator<String> strIter = hm.keySet().iterator();
		while(strIter.hasNext()) {
			String tt = strIter.next();
			System.out.println(tt+":"+hm.get(tt));
		}
	}
}
