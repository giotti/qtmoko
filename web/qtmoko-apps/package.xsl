<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="html"/>

<xsl:template match="package">
<html>
<head>
<title><xsl:apply-templates select="@title"/></title>
</head>
<body>
<center>
<h1><xsl:apply-templates select="@title"/></h1>

<xsl:element name="a">
<xsl:attribute name="href">
<xsl:apply-templates select="@name"/>
<xsl:text>_</xsl:text>
<xsl:apply-templates select="@version"/>
<xsl:text>_</xsl:text>
<xsl:choose>
  <xsl:when test="@arch">
    <xsl:apply-templates select="@arch"/>
  </xsl:when>
  <xsl:otherwise>
    <xsl:text>armel</xsl:text>
  </xsl:otherwise>
</xsl:choose>
<xsl:text>.deb</xsl:text>
</xsl:attribute>
<xsl:element name="img">
<xsl:attribute name="src">
<xsl:apply-templates select="@name"/>
<xsl:text>.png</xsl:text>
</xsl:attribute>
<xsl:attribute name="width">120</xsl:attribute>
<xsl:attribute name="height">180</xsl:attribute>
<xsl:attribute name="border">0</xsl:attribute>
</xsl:element>
</xsl:element>

<p><xsl:apply-templates select="@summary"/></p>

<xsl:element name="a">
<xsl:attribute name="href">
<xsl:apply-templates select="@name"/>
<xsl:text>_</xsl:text>
<xsl:apply-templates select="@version"/>
<xsl:text>_</xsl:text>
<xsl:choose>
  <xsl:when test="@arch">
    <xsl:apply-templates select="@arch"/>
  </xsl:when>
  <xsl:otherwise>
    <xsl:text>armel</xsl:text>
  </xsl:otherwise>
</xsl:choose>
<xsl:text>.deb</xsl:text>
</xsl:attribute>
<xsl:text>Click here to install</xsl:text>
</xsl:element>

<xsl:apply-templates select="details"/>


</center>
</body>
</html>
</xsl:template>

<xsl:template match="@title">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="@summary">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="@name">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="@version">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="details">
 <xsl:copy-of select="./*"/>
</xsl:template>

</xsl:stylesheet>