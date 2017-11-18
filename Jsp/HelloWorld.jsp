<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
<h1>hello liucong</h1>
<%!
	String s = "张三";
	int add(int b, int e) {
		return b+e;
	}

	
%>

<%--返回九九乘法表 --%>
<%!
//返回九九乘法表
String printMultitable1() {
	String s="";
	for(int i = 1;i<=9;i++) {
		for(int j = 1;j<=i;j++) {
			s+=i+"*"+j+"="+(i*j)+"&nbsp;&nbsp;&nbsp";
		}
		s+="<br/>";
	}
	return s;		
}

void printMultitable2(JspWriter out) throws Exception{
	for(int i = 1;i<=9;i++) {
		for(int j = 1;j<=i;j++) {
			out.println(i+"*"+j+"="+(i*j)+"&nbsp;&nbsp;&nbsp");
		}
		out.println("<br/>");
	}
}
%>

<h1>九九乘法表</h1>
<%=printMultitable1() %>
<% printMultitable2(out); %>
<hr>
<%
out.println("欢迎");

%>
<hr>
hello <%=s %><br/>
x+y = <%=add(10,5) %>


</body>
</html>