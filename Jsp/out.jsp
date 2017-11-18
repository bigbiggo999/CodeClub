<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
<h1>out内置对象</h1>

<%
out.println("<h2>jingyesi</h2>");
out.flush();
out.clearBuffer();//out.flush()之后不能用out.clear();
%>

缓冲区大小: <%=out.getBufferSize() %>byte<br>
bufferremaining: <%=out.getRemaining() %>byte<br>
是否自动清空：<%=out.isAutoFlush() %><br>



</body>
</html>