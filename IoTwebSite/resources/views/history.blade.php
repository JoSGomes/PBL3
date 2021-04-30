@extends('layouts.template')

@section('title')
    Histórico
@endsection

@section('body')
<div class="subtitle">Histórico de eventos</div>
<table class="table table-hover table-bordeless">
    <thead>
    <tr>
        <th scope="col">#</th>
        <th scope="col">Horário</th>
        <th scope="col">Evento</th>
    </tr>
    </thead>
    <tbody>
    <tr >
        <th scope="row">1</th>
        <td>Mark</td>
        <td>Otto</td>
    </tr>
    </tbody>
</table>
@endsection